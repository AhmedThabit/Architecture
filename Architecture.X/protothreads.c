#include <stdint.h>       // for uint32_t
#include "../../pt.h"
#include "../Architecture.X/protothreads.h"
#include "definitions.h"                // SYS function prototypes
#include "bsp.h"

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include "esp32_proto.h"
#include <stdio.h>
#include "ff.h"
#include "spi_bus_guard.h"

#define UART1_DEBUG 0
#define UART1_THREAD 0


/* --- Config --- */
#define SMS_NUMBER   "+201121844048"       /* <-- change me */
#define SMS_TEXT     "Hello from PIC32 via UART3 ?\r\n"
#define SMS_PERIOD_MS   100000UL             /* 100 seconds */
#define AT_TIMEOUT(ms)  (ms)

/* --- Minimal RX accumulator for token waits --- */
static char g_rx_acc[512];
static uint16_t g_rx_len;

extern bool ESP32_TakeRxFlag(void); // if you added it; else just call ESP32_Poll()

// Millisecond tick, incremented in Timer1 ISR
extern volatile uint32_t msTicks;

extern volatile bool sms_enabled = true;
extern volatile unsigned sms_count = 1;

static FATFS fs;
static bool sd_mounted = false;

uint8_t sms_get_enabled(void) {
    return sms_enabled ? 1u : 0u;
}

// One pt struct per thread
struct pt ptSensor, ptTelit, ptEsp32, ptEth, ptCLI, ptEspTxTest, ptPreflight, ptSdCard;

void handle_sms_enable_cmd(uint8_t flag) {
    sms_enabled = (flag != 0);
    sms_count = 0; // optional: reset attempts
}

void UART1_SendChar11(char c) {
    while (U1STAbits.UTXBF);
    U1TXREG = c;
}

void UART1_WriteString11(const char *str) {
    while (*str) UART1_SendChar11(*str++);
}

void UART3_WriteString33(const char* str) {
    UART3_Write((void*) str, strlen(str));
}

void BlockingUART3_WriteString33(const char* s) {
    while (*s) {
        while (U3STAbits.UTXBF); // wait for space
        U3TXREG = *s++;
    }
}

void UART3_WriteChar33(char c) {
    while (U3STAbits.UTXBF); // Wait if TX buffer is full
    U3TXREG = c;
}

// ------------- uart 3 --- sms

/* Append any pending UART3 RX bytes into the accumulator */
static void rx_accumulate(void) {
    uint8_t tmp[128];
    int n;
    while ((n = UART3_Read(tmp, sizeof (tmp))) > 0) {
        if (g_rx_len + n >= sizeof (g_rx_acc))
            g_rx_len = 0; /* simple overflow recovery */
        memcpy(&g_rx_acc[g_rx_len], tmp, (size_t) n);
        g_rx_len += (uint16_t) n;
        g_rx_acc[g_rx_len] = '\0';
    }
}

/* Start a new wait window */
static void rx_wait_begin(uint32_t *t0) {
    g_rx_len = 0;
    g_rx_acc[0] = '\0';
    *t0 = msTicks;
}

/* Return true when either token found OR timeout elapsed.
   Set *found=true only if token found. */
static bool rx_wait_token(const char *tok, uint32_t t0, uint32_t timeout_ms, bool *found) {
    rx_accumulate();
    if (strstr(g_rx_acc, tok)) {
        *found = true;
        return true;
    }
    if (msTicks - t0 >= timeout_ms) {
        *found = false;
        return true;
    }
    return false;
}

/* Same as above but succeeds if ANY of the tokens is seen; returns which index via *which (-1 on timeout) */
static bool rx_wait_any(const char *const toks[], int ntoks, uint32_t t0, uint32_t timeout_ms, int *which) {
    rx_accumulate();
    for (int i = 0; i < ntoks; i++) {
        if (strstr(g_rx_acc, toks[i])) {
            *which = i;
            return true;
        }
    }
    if (msTicks - t0 >= timeout_ms) {
        *which = -1;
        return true;
    }
    return false;
}

/* === helpers (put near your other RX helpers) === */
static bool rx_wait_cpin(uint32_t t0, uint32_t timeout_ms, int *which) {
    rx_accumulate();

    char *p = strstr(g_rx_acc, "+CPIN:");
    if (p) {
        p += 6; // past "+CPIN:"
        while (*p == ' ' || *p == '\t') p++; // skip spaces

        // grab token to end-of-line
        const char *s = p;
        while (*p && *p != '\r' && *p != '\n') p++;
        size_t n = (size_t) (p - s);
        while (n && (s[n - 1] == ' ' || s[n - 1] == '\t')) n--; // trim

        int res = 3; // other/error
        if (n == 5 && !memcmp(s, "READY", 5)) res = 0;
        else if (n == 7 && !memcmp(s, "SIM PIN", 7)) res = 1;
        else if (n == 7 && !memcmp(s, "SIM PUK", 7)) res = 2;

        if (which) *which = res;

        // consume the whole line (and its CRLF)
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        size_t cut = (size_t) (p - g_rx_acc);
        memmove(g_rx_acc, g_rx_acc + cut, g_rx_len + 1 - cut);
        g_rx_len -= (g_rx_len >= cut) ? cut : g_rx_len;

        return true;
    }

    if ((uint32_t) (msTicks - t0) >= timeout_ms) {
        if (which) *which = -1;
        return true;
    }
    return false;
}

static bool rx_wait_creg_ok(uint32_t t0, uint32_t timeout_ms, bool *ok_out) {
    rx_accumulate();
    char *p = strstr(g_rx_acc, "+CREG:");
    if (p) {
        // consume that whole line
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        size_t cut = (size_t) (p - g_rx_acc);
        // decide 1/5 = registered
        bool ok = (strstr(g_rx_acc, ",1") || strstr(g_rx_acc, ",5")) != NULL;
        if (ok_out) *ok_out = ok;
        memmove(g_rx_acc, g_rx_acc + cut, g_rx_len + 1 - cut);
        g_rx_len -= (g_rx_len >= cut) ? cut : g_rx_len;
        return true;
    }
    if ((uint32_t) (msTicks - t0) >= timeout_ms) {
        if (ok_out) *ok_out = false;
        return true;
    }
    return false;
}

/* Send Ctrl+Z */
static inline void uart3_ctrl_z(void) {
    // end the SMS body:
    UART3_WriteChar33((char) 0x1A); // DO NOT send CR/LF after this

}

static void uart3_write_hex3(uint8_t byte) {
    const char hex[] = "0123456789ABCDEF";
    char out[3] = {hex[byte >> 4], hex[byte & 0x0F], 0};
    BlockingUART3_WriteString33(out);
}


//-------------end uart 3 ---- sms

static const char* fatfs_err_str(FRESULT r) {
    switch (r) {
        case FR_OK: return "FR_OK";
        case FR_NOT_READY: return "FR_NOT_READY";
        case FR_NO_FILESYSTEM: return "FR_NO_FILESYSTEM";
        case FR_DISK_ERR: return "FR_DISK_ERR";
        case FR_TIMEOUT: return "FR_TIMEOUT";
        default: return "FR_OTHER";
    }
}

//static bool SD_MountStep(void) {
//    UART3_WriteString33("SD mount step...\r\n");
//
//    // 1) Register FS object (should be fast)
//    FRESULT res = f_mount(&fs, "0:", 0);
//    if (res != FR_OK) {
//        sd_mounted = false;
//        UART3_WriteString33("f_mount failed: ");
//        UART3_WriteString33(fatfs_err_str(res));
//        UART3_WriteString33("\r\n");
//        return false;
//    }
//
//    // 2) Trigger media access with a fast verification call
//    DWORD fre_clust = 0;
//    FATFS *pfs = NULL;
//    res = f_getfree("0:", &fre_clust, &pfs);
//
//    if (res == FR_OK) {
//        sd_mounted = true;
//        UART3_WriteString33("SD mounted OK\r\n");
//        return true;
//    }
//
//    sd_mounted = false;
//    UART3_WriteString33("SD not ready: ");
//    UART3_WriteString33(fatfs_err_str(res));
//    UART3_WriteString33("\r\n");
//    return false;
//}

#include "sd_fatfs_guard.h"

//static bool SD_Mount(void) {
//    BlockingUART3_WriteString33("SD mount start...\r\n");
//
//    // deselect flash (shared bus)
//    GPIO_RA9_FL_SS3_Set();
//
//    BlockingUART3_WriteString33("f_mount...\r\n");
//    FRESULT res = SD_f_mount(&fs, "0:", 0); //f_mount(&fs, "0:", 0);
//    BlockingUART3_WriteString33("f_mount returned\r\n");
//
//    if (res != FR_OK) {
//        BlockingUART3_WriteString33("f_mount FAIL: ");
//        BlockingUART3_WriteString33(fatfs_err_str(res));
//        BlockingUART3_WriteString33("\r\n");
//        sd_mounted = false;
//        return false;
//    }
//
//    BlockingUART3_WriteString33("f_getfree...\r\n");
//    DWORD fre_clust = 0;
//    FATFS *pfs = 0;
//    res = f_getfree("0:", &fre_clust, &pfs);
//    BlockingUART3_WriteString33("f_getfree returned\r\n");
//
//    if (res == FR_OK) {
//        BlockingUART3_WriteString33("SD mounted OK\r\n");
//        sd_mounted = true;
//        return true;
//    }
//
//    BlockingUART3_WriteString33("SD verify FAIL: ");
//    BlockingUART3_WriteString33(fatfs_err_str(res));
//    BlockingUART3_WriteString33("\r\n");
//    sd_mounted = false;
//    return false;
//}




//PorotoThread

void Protothreads_Init(void) {
    PT_INIT(&ptSensor);
    PT_INIT(&ptTelit);
    PT_INIT(&ptEsp32);
    PT_INIT(&ptEspTxTest);
    PT_INIT(&ptEth);
    PT_INIT(&ptCLI);
    PT_INIT(&ptPreflight);
    PT_INIT(&ptSdCard);


}


/* ????????? SensorThread ????????? */
PT_THREAD(SensorThread(struct pt *pt)) {
    static uint32_t t0;
    PT_BEGIN(pt);

    while (1) {
        t0 = msTicks;
        //        HAL_ADC_StartConversion();
        //        PT_WAIT_UNTIL(pt, HAL_ADC_ConversionComplete());
        //        uint16_t val = HAL_ADC_GetResult();
        //        processSensor(val);                // your handler




#if UART1_THREAD
        UART1_WriteString11("Sensor!\n\r");
#endif


        PT_WAIT_UNTIL(pt, UART1_TransmitComplete());

        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);

    }

    PT_END(pt);
}

PT_THREAD(TelitThread(struct pt *pt)) {
    static uint32_t t0, last_send;
    static bool ok;
    static int which;

    PT_BEGIN(pt);
    PT_WAIT_UNTIL(pt, msTicks >= 10001);
    /* One-time: put the modem into SMS text mode */
    /* "AT" -> expect OK */
    UART3_WriteString33("AT\r\n");
    rx_wait_begin(&t0);
    PT_WAIT_UNTIL(pt, rx_wait_token("OK", t0, AT_TIMEOUT(1500), &ok));
    /* If not OK, continue; loop will try again next period */

    /* "AT+CMGF=1" -> OK (SMS text mode) */
    UART3_WriteString33("AT+CMGF=1\r\n");
    rx_wait_begin(&t0);

    PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
        "OK", "ERROR"
    }, 2, t0, AT_TIMEOUT(2000), &which));

    last_send = msTicks;

    while (1) {
        /* Keep the modem awake / responsive (optional ping) */
#if UART1_THREAD
        UART1_WriteString11("Telit!\n\r");
#endif
        UART3_WriteString33("AT\r\n");
        rx_wait_begin(&t0);

        PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
            "OK", "ERROR"
        }, 2, t0, AT_TIMEOUT(1000), &which));
#if UART1_THREAD
        PT_WAIT_UNTIL(pt, UART1_TransmitComplete());
#endif

        /* Wait until 10s elapsed since last SMS */
        PT_WAIT_UNTIL(pt, (msTicks - last_send) >= SMS_PERIOD_MS);

        /* ======= SEND ONE SMS =======888 */
        /* gate by flags */
        if (!sms_enabled || sms_count >= 3) { // cap attempts if you want
            last_send = msTicks;
            PT_YIELD(pt);
            continue;
        }

        /* 1) Issue CMGS with the destination number */
        {
            UART3_WriteString33("AT\r\n");
            rx_wait_begin(&t0);

            PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
                "OK", "ERROR"
            }, 2, t0, AT_TIMEOUT(1000), &which));

            /* 1) Issue CMGS with the destination number */
            const char* dest = phonebook_get_number(phonebook_get_default());
            if (!dest) {
                // No default number set ? skip or log error
                last_send = msTicks;
                continue;
            }

            char cmd[64];
            snprintf(cmd, sizeof (cmd), "AT+CMGS=\"%s\"\r\n", dest);
            //snprintf(cmd, sizeof (cmd), "AT+CMGS=\"%s\"\r\n", SMS_NUMBER);
            UART3_WriteString33(cmd);
        }

        /* 2) Wait for prompt ('>' or 'CONNECT'), or bail on 'ERROR' */
        rx_wait_begin(&t0);

        PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
            ">", "CONNECT", "ERROR"
        }, 3, t0, AT_TIMEOUT(8000), &which));
        if (which == 2) {
            /* ERROR: skip this cycle, try again next period */
            last_send = msTicks;
            continue;
        }

        /* 3) Send the SMS body, then Ctrl+Z to submit */
        UART3_WriteString33(SMS_TEXT);

        /* ensure ALL bytes actually left the UART before sending 0x1A */
        PT_WAIT_UNTIL(pt, UART3_TransmitComplete());

        uart3_ctrl_z();

        /* 4) Wait for +CMGS and final OK (tolerate modem chatter) */
        {
            uint8_t seen_cmgs = 0;
            uint32_t t_cmgs = msTicks;
            while ((msTicks - t_cmgs) < AT_TIMEOUT(30000)) {
                /* pump RX and check tokens each pass */
                rx_accumulate();
                if (!seen_cmgs && strstr(g_rx_acc, "+CMGS")) {
                    seen_cmgs = 1;
                }
                if (strstr(g_rx_acc, "OK")) {
                    /* delivered */
                    break;
                }
                if (strstr(g_rx_acc, "ERROR")) {
                    /* failed, break early */
                    break;
                }
                PT_YIELD(pt); /* don?t block CPU; yield until next tick */
            }
        }
        sms_count = sms_count + 1;
        /* Mark the time of this attempt; loop will send again after 10s */
        last_send = msTicks;

        /* Optional little delay before next loop body */
        t0 = msTicks;
        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);
    }

    PT_END(pt);
}

PT_THREAD(TelitPreflightThread(struct pt *pt)) {
    static uint32_t t0, t1;
    static int cpin, which;
    static bool ok;

    PT_BEGIN(pt);

    /* Echo OFF (less junk in buffer) */
    rx_wait_begin(&t0);
    UART3_WriteString33("AT\r");
    PT_WAIT_UNTIL(pt, rx_wait_token("OK", t0, 1000, NULL));

    /* Verbose errors */
    rx_wait_begin(&t0);
    UART3_WriteString33("AT+CMEE=2\r");
    PT_WAIT_UNTIL(pt, rx_wait_token("OK", t0, 1000, NULL));

    /* SIM ready?  poll AT+CPIN? until READY */
    for (;;) {
        rx_wait_begin(&t0); // <-- open window FIRST
        UART3_WriteString33("AT+CPIN?\r");
        PT_WAIT_UNTIL(pt, rx_wait_cpin(t0, 4000, &cpin));
        if (cpin == 0) break; // 0=READY (from rx_wait_cpin)
        t1 = msTicks;
        PT_WAIT_UNTIL(pt, (msTicks - t1) >= 500);
    }

    /* Registered on network? poll CREG until ,1 or ,5 */
    for (;;) {
        rx_wait_begin(&t0);
        UART3_WriteString33("AT+CREG?\r");
        PT_WAIT_UNTIL(pt, rx_wait_creg_ok(t0, 2000, &ok));
        if (ok) break;
        t1 = msTicks;
        PT_WAIT_UNTIL(pt, (msTicks - t1) >= 1000);
    }

    /* SMS text mode */
    rx_wait_begin(&t0);
    UART3_WriteString33("AT+CMGF=1\r");

    PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
        "OK", "+CMS ERROR"
    }, 2, t0, 3000, &which));

    /* GSM charset (optional) */
    rx_wait_begin(&t0);
    UART3_WriteString33("AT+CSCS=\"GSM\"\r");

    PT_WAIT_UNTIL(pt, rx_wait_any((const char*[]) {
        "OK", "+CME ERROR", "+CMS ERROR"
    }, 3, t0, 3000, &which));

    PT_END(pt);
}



//
///* ????????? TelitThread ?????????? */
//PT_THREAD(TelitThread(struct pt *pt)) {
//    static uint32_t t0,t1;
//    PT_BEGIN(pt);
//    t1 = msTicks;
//    PT_WAIT_UNTIL(pt, (msTicks - t1) >= 10000);   // 10,000 ms
//    
//    while (1) {
//        t0 = msTicks;
//        //        PT_WAIT_UNTIL(pt, telitDataReady());
//        //        uint8_t buf[128];
//        //        int len = UART3_Read(buf, sizeof(buf));
//        //        handleTelitResponse(buf, len);
//
//#if UART1_THREAD
//        UART1_WriteString11("Telit!\n\r");
//#endif
//
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 10000);
//        UART3_WriteString33("AT\r\n");
//        
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 10020);
//        
//        
//        UART3_WriteString33("AT+CMGF=1\r\n");
//        
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 10070);
//        
//        /* 1) Issue CMGS with the destination number */
//        {
//            char cmd[64];
//            snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"\r\n", SMS_NUMBER);
//            UART3_WriteString33(cmd);
//        }
//        
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 10270);
//        
//        UART3_WriteString33(SMS_TEXT);
//        
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 10470);
//        
//        uart3_ctrl_z();
//        
//        //PT_WAIT_UNTIL(pt, UART1_TransmitComplete());
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 11000);
//
//    }
//
//    PT_END(pt);
//}

PT_THREAD(Esp32TxTestThread(struct pt *pt)) {
    static uint32_t t0;
    PT_BEGIN(pt);
    while (1) {
        t0 = msTicks;
        const uint8_t payload[] = "PIC32 HELLO";
        ESP32_SendFrame(payload, sizeof (payload) - 1);
        PT_WAIT_UNTIL(pt, (uint32_t) (msTicks - t0) >= 2000);
    }
    PT_END(pt);
}

PT_THREAD(Esp32Thread(struct pt *pt)) {
    PT_BEGIN(pt);

    // Do this ONCE (or move both calls to main() right after SYS_Initialize)
    static bool inited = false;
    if (!inited) {
        ESP32_UartInit();
        ESP32_RegisterFrameHandler(Esp_HandleFrame);
        inited = true;
    }

    while (1) {
        // Wait until at least 1 byte is in UART1 RX ring
        PT_WAIT_UNTIL(pt, UART1_ReadCountGet() > 0);

        // Drain and assemble frames (will call Esp_HandleFrame on a full, valid one)
        ESP32_Poll();

        // Yield cooperatively so others can run
        PT_YIELD(pt);
    }

    PT_END(pt);
}

///* ????????? Esp32Thread ????????? */
//PT_THREAD(Esp32Thread(struct pt *pt)) {
//    static uint32_t t0;
//    PT_BEGIN(pt);
//    ESP32_UartInit();
//    // Register app-level handler once
//    ESP32_RegisterFrameHandler(Esp_HandleFrame); // you implement this
//
//    while (1) {
//        t0 = msTicks;
//        //        PT_WAIT_UNTIL(pt, esp32DataReady());
//        //        uint8_t buf[128];
//        //        int len = UART1_Read(buf, sizeof (buf));
//        // === UART test = One-time UART startup messages  ===
//        //UART1_Write((uint8_t *) "ESP32!\n", sizeof ("ESP32!\n") + 16);
//
//        // wait for RX or add a timed poll
//        PT_WAIT_UNTIL(pt, /* rx flag from Option B */ true);
//        ESP32_Poll();
//
//        UART1_WriteString11("ESP32!\n\r");
//        PT_WAIT_UNTIL(pt, UART1_TransmitComplete());
//        // protothreads.c (Esp32Thread)
////        PT_WAIT_UNTIL(pt, ESP32_TakeRxFlag() /* or timeout condition */);
//
//        UART3_Write((uint8_t *) "PT AT\r\n", 4);
//        //        handleEsp32(buf, len);
//        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);
//    }
//
//    PT_END(pt);
//}

/* ????????? EthThread ???????????? */
PT_THREAD(EthThread(struct pt *pt)) {
    static uint32_t t0;
    PT_BEGIN(pt);

    while (1) {
        t0 = msTicks;
        //        ETH_PeriodicTasks();    // e.g. lwIP timers or packet handling

#if UART1_THREAD
        UART1_WriteString11("EtherNet!\n\r");
#endif

        PT_WAIT_UNTIL(pt, UART1_TransmitComplete());
        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);
        //        PT_YIELD(pt); // give other threads a chance
    }

    PT_END(pt);
}

/* ????????? CliThread ??????????? */
PT_THREAD(CliThread(struct pt *pt)) {
    static uint32_t t0;
    PT_BEGIN(pt);

    while (1) {
        t0 = msTicks;

#if UART1_THREAD
        UART1_WriteString11("Cli!\n\r");
#endif

        PT_WAIT_UNTIL(pt, UART1_TransmitComplete());


        //        PT_WAIT_UNTIL(pt, cliInputReady());
        //        char cmd[64];
        //        cliReadLine(cmd, sizeof(cmd));
        //        cliExecute(cmd);
        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);
    }

    PT_END(pt);
}









/***
 future needs
 * 
 * 
 * Totally doable. You?ve got a few clean ways to ?skip? a protothread based on a condition?pick what fits your flow.

1) Skip at the scheduler (simplest)
Don?t call the thread when it?s disabled.

c
Copy
Edit
extern volatile bool telit_enabled;

while (1) {
    SYS_Tasks();

    PT_SCHEDULE(SensorThread(&ptSensor));
    if (telit_enabled) PT_SCHEDULE(TelitThread(&ptTelit));   // <- gate here
    PT_SCHEDULE(Esp32Thread(&ptEsp32));
    PT_SCHEDULE(EthThread(&ptEth));
    PT_SCHEDULE(CliThread(&ptCLI));
}
Pros: zero CPU inside that PT when disabled.

If telit_enabled can change in an ISR, declare it volatile.

2) Block at the top of the thread
Make the thread sleep until enabled.

PT_THREAD(TelitThread(struct pt *pt))
{
    PT_BEGIN(pt);
    while (1) {
        PT_WAIT_UNTIL(pt, telit_enabled);   // parked until true

        // ... do work while enabled ...

        // Optional: go idle again when disabled
        PT_WAIT_UNTIL(pt, !telit_enabled);
    }
    PT_END(pt);
}
Pros: very clear. The PT yields while disabled.

3) Skip just this iteration
If you only want to bypass the work for one loop pass:

while (1) {
    if (!telit_enabled) { PT_YIELD(pt); continue; }  // quick skip

    // normal body
    // ...
}
PT_YIELD cooperatively gives time to others, then you re-check next call.

4) Hard stop / cancel the thread
Terminate the PT until you explicitly reinit it.

if (!telit_enabled) {
    // optional: cleanup here
    PT_EXIT(pt);            // thread is now ?dead?
}
/* later, to re-enable: *
PT_INIT(&ptTelit);
Use this if you want a fresh start (state reset) when re-enabling.

5) Semaphore-style pause/resume (built-in to Protothreads)
If you prefer signaling:

#include "pt-sem.h"
static struct pt_sem telit_sem;

// init once
PT_SEM_INIT(&telit_sem, 0);

// thread
PT_SEM_WAIT(pt, &telit_sem);   // blocks here until signaled
// ...work...

// somewhere else to resume it:
PT_SEM_SIGNAL(pt_any, &telit_sem);
Tips
If the condition is set from an ISR, make it volatile bool and keep ISR work minimal.

If you disable a thread that might be mid-operation (e.g., waiting on UART), add the condition to your waits:

PT_WAIT_UNTIL(pt, UART1_TransmitComplete() || !telit_enabled);
if (!telit_enabled) { // optional abort/cleanup 
                        continue; }
If you tell me which thread(s) you want to gate and what the flag is (e.g., a CLI command), I can wire the exact pattern into your current code.
 
 */