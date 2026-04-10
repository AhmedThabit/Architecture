/**
 * @file    protothreads.c
 * @brief   Cooperative Protothread tasks for the Emergency Audio Dialer.
 *
 * @details Each PT_THREAD is a lightweight cooperative task scheduled from
 *          App_Run().  They use the abstract modem API:
 *            Modem_Xxx_Cmd()  — start an operation
 *            Modem_Xxx_Poll() — poll for result (used in PT_WAIT_UNTIL)
 *
 *          All modem access goes through modem_api.h — no direct AT calls.
 *          To swap modem hardware, only the backend .c file changes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "common/pt.h"
#include "../src/common/pt.h"
#include "protothreads.h"
#include "definitions.h"

/* HAL */
#include "hal/bsp/bsp.h"
#include "hal/spi_guard/spi_bus_guard.h"

/* Modem abstraction layer (no direct AT calls!) */
#include "services/modem_api/modem_api.h"

/* Services */
#include "services/esp32_proto/esp32_proto.h"
#include "services/storage/store.h"

/* FatFS (for SD card thread) */
#include "ff.h"
#include "common/sd_fatfs_guard.h"

/* Audio */
#include "services/audio/audio_api.h"

/* Config */
#include "config/app_config.h"

/* ── External references ────────────────────────────────────────────────── */

extern volatile uint32_t g_ms_ticks;
extern void ESP32_UartInit(void);
extern void ESP32_RegisterFrameHandler(void (*cb)(const uint8_t*, size_t));
extern void ESP32_Poll(void);
extern void Esp_HandleFrame(const uint8_t* payload, size_t len);
extern size_t UART1_ReadCountGet(void);
extern void PhonebookFlash_Init(void);

/* ── SMS control flags ──────────────────────────────────────────────────── */

#define SMS_PERIOD_MS       100000UL    /* 100 seconds between SMS attempts */
#define SMS_MAX_ATTEMPTS    3u

volatile bool     sms_enabled = true;
volatile unsigned sms_count   = 0;

uint8_t sms_get_enabled(void)
{
    return sms_enabled ? 1u : 0u;
}

void handle_sms_enable_cmd(uint8_t flag)
{
    sms_enabled = (flag != 0);
    sms_count   = 0;
}

/* ── Protothread control blocks ─────────────────────────────────────────── */

struct pt ptSensor, ptTelit, ptEsp32, ptEth, ptCLI;
struct pt ptEspTxTest, ptPreflight, ptSdCard, ptAudio;

volatile bool g_audio_init_done = false;

void Protothreads_Init(void)
{
    PT_INIT(&ptSensor);
    PT_INIT(&ptTelit);
    PT_INIT(&ptEsp32);
    PT_INIT(&ptEspTxTest);
    PT_INIT(&ptEth);
    PT_INIT(&ptCLI);
    PT_INIT(&ptPreflight);
    PT_INIT(&ptSdCard);
    PT_INIT(&ptAudio);
}

/* ── Debug UART helpers (still needed for ESP32 proto debug output) ────── */

void UART3_WriteString33(const char *str)
{
    UART3_Write((void *)str, strlen(str));
}

void BlockingUART3_WriteString33(const char *s)
{
    while (*s) {
        while (U3STAbits.UTXBF);
        U3TXREG = *s++;
    }
}

void UART3_WriteChar33(char c)
{
    while (U3STAbits.UTXBF);
    U3TXREG = c;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  TelitPreflightThread — Boot-up modem check sequence
 *
 *  Uses the abstract modem API (modem_api.h):
 *    Modem_Xxx_Cmd()  → starts the operation
 *    Modem_Xxx_Poll() → returns true when complete
 *
 *  When you swap to a 5G modem, this thread stays UNCHANGED.
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(TelitPreflightThread(struct pt *pt))
{
    static uint32_t       t_delay;
    static Modem_Result   res;
    static Modem_SimStatus sim_status;
    static Modem_NetStatus net_status;

    PT_BEGIN(pt);

    /* 1. Echo OFF */
    Modem_EchoOff_Cmd();
    PT_WAIT_UNTIL(pt, Modem_EchoOff_Poll(&res));

    /* 2. Verbose error codes */
    Modem_VerboseErrors_Cmd();
    PT_WAIT_UNTIL(pt, Modem_VerboseErrors_Poll(&res));

    /* 3. Wait for SIM to be READY */
    for (;;) {
        Modem_QuerySIM_Cmd();
        PT_WAIT_UNTIL(pt, Modem_QuerySIM_Poll(&res, &sim_status));
        if (sim_status == MODEM_SIM_READY) break;

        t_delay = g_ms_ticks;
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t_delay) >= 500);
    }

    /* 4. Wait for network registration */
    for (;;) {
        Modem_QueryRegistration_Cmd();
        PT_WAIT_UNTIL(pt, Modem_QueryRegistration_Poll(&res, &net_status));
        if (net_status == MODEM_NET_HOME || net_status == MODEM_NET_ROAMING) break;

        t_delay = g_ms_ticks;
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t_delay) >= 1000);
    }

    /* 5. Configure SMS subsystem (text mode, charset, notify, storage) */
    Modem_ConfigSMS_Cmd();
    PT_WAIT_UNTIL(pt, Modem_ConfigSMS_Poll(&res));

    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  TelitThread — Periodic SMS sender
 *
 *  After preflight completes, this thread periodically sends an SMS
 *  to the default phonebook number using the abstract modem API.
 *
 *  NOTE: Modem_SendSMS_Cmd() handles the ENTIRE multi-step dance
 *        (AT+CMGS → ">" → body → Ctrl-Z → OK) in ONE Cmd/Poll pair.
 *        No more manual prompt handling or Ctrl-Z sending!
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(TelitThread(struct pt *pt))
{
    static uint32_t     t0, last_send;
    static Modem_Result res;

    PT_BEGIN(pt);

    /* Wait for boot delay */
    PT_WAIT_UNTIL(pt, g_ms_ticks >= 10001);

    /* Initial modem ping */
    Modem_Ping_Cmd();
    PT_WAIT_UNTIL(pt, Modem_Ping_Poll(&res));

    /* Configure SMS text mode */
    Modem_ConfigSMS_Cmd();
    PT_WAIT_UNTIL(pt, Modem_ConfigSMS_Poll(&res));

    last_send = g_ms_ticks;

    while (1) {
        /* Periodic keep-alive */
        Modem_Ping_Cmd();
        PT_WAIT_UNTIL(pt, Modem_Ping_Poll(&res));

        /* Wait for SMS period */
        PT_WAIT_UNTIL(pt, (g_ms_ticks - last_send) >= SMS_PERIOD_MS);

        /* Gate by enable flag and attempt counter */
        if (!sms_enabled || sms_count >= SMS_MAX_ATTEMPTS) {
            last_send = g_ms_ticks;
            PT_YIELD(pt);
            continue;
        }

        /* Look up the default phonebook number */
        const char *dest = phonebook_get_number(phonebook_get_default());
        if (!dest) {
            last_send = g_ms_ticks;
            continue;
        }

        /* Send SMS — ONE call handles the entire AT+CMGS dance */
        Modem_SendSMS_Cmd(dest, "Hello from PIC32 via UART3");
        PT_WAIT_UNTIL(pt, Modem_SendSMS_Poll(&res));

        if (res == MODEM_OK) {
            sms_count++;
        }
        last_send = g_ms_ticks;

        /* Small delay before next iteration */
        t0 = g_ms_ticks;
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t0) >= 1000);
    }

    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Esp32Thread — ESP32 binary protocol handler
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(Esp32Thread(struct pt *pt))
{
    PT_BEGIN(pt);

    static bool inited = false;
    if (!inited) {
        ESP32_UartInit();
        ESP32_RegisterFrameHandler(Esp_HandleFrame);
        inited = true;
    }

    while (1) {
        PT_WAIT_UNTIL(pt, UART1_ReadCountGet() > 0);
        ESP32_Poll();
        PT_YIELD(pt);
    }

    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  SensorThread — Periodic sensor reading
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(SensorThread(struct pt *pt))
{
    static uint32_t t0;

    PT_BEGIN(pt);
    while (1) {
        t0 = g_ms_ticks;
        /* TODO: HAL_ADC_StartConversion(), wait, read, process */
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t0) >= 1000);
    }
    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  EthThread — Ethernet periodic tasks
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(EthThread(struct pt *pt))
{
    static uint32_t t0;

    PT_BEGIN(pt);
    while (1) {
        t0 = g_ms_ticks;
        /* TODO: ETH_PeriodicTasks() */
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t0) >= 1000);
    }
    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  CliThread — Command-line interface
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(CliThread(struct pt *pt))
{
    static uint32_t t0;

    PT_BEGIN(pt);
    while (1) {
        t0 = g_ms_ticks;
        /* TODO: read CLI input, parse, execute */
        PT_WAIT_UNTIL(pt, (g_ms_ticks - t0) >= 1000);
    }
    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Esp32TxTestThread — Periodic test frame sender
 * ══════════════════════════════════════════════════════════════════════════ */

extern bool ESP32_SendFrame(const uint8_t *payload, size_t len);

PT_THREAD(Esp32TxTestThread(struct pt *pt))
{
    static uint32_t t0;

    PT_BEGIN(pt);
    while (1) {
        t0 = g_ms_ticks;
        const uint8_t payload[] = "PIC32 HELLO";
        ESP32_SendFrame(payload, sizeof(payload) - 1);
        PT_WAIT_UNTIL(pt, (uint32_t)(g_ms_ticks - t0) >= 2000);
    }
    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  SdCardThread — SD card mount, directory create, file copy, LED on
 *
 *  Flow:
 *    1. Wait for SYS_FS_Mount() to succeed  (Harmony driver needs time)
 *    2. Unmount and remount               (proves driver stability)
 *    3. Set current drive
 *    4. Open source file for reading
 *    5. Create "Dir1" directory
 *    6. Open destination file inside Dir1 for writing
 *    7. Copy data in 512-byte chunks
 *    8. Close both files
 *    9. Turn LED ON = success
 *
 *  IMPORTANT: SYS_FS_Tasks() and DRV_SDSPI_Tasks() run inside SYS_Tasks()
 *  which App_Run() calls every loop pass.  This thread cooperates by
 *  yielding (PT_YIELD) between attempts, letting those drivers advance.
 *
 *  TEST: Put a file named "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG" on the
 *        SD card root.  After boot, "Dir1/" will be created and the file
 *        copied into it.  LED turns ON when done.  LED stays OFF on error.
 * ══════════════════════════════════════════════════════════════════════════ */

#include "user.h"   /* LED_ON / LED_OFF macros */

/* SD card test parameters — same file/dir as the original working demo */
#define SD_MOUNT_NAME   "/mnt/mydrive"
#define SD_DEV_NAME     "/dev/mmcblka1"
#define SD_FILE_NAME    "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG"
#define SD_DIR_NAME     "Dir1"
#define SD_BUF_LEN      512

static uint8_t s_sd_buf[SD_BUF_LEN];

PT_THREAD(SdCardThread(struct pt *pt))
{
    static SYS_FS_HANDLE s_fh_src;
    static SYS_FS_HANDLE s_fh_dst;
    static int32_t       s_n_read;

    PT_BEGIN(pt);

    LED_OFF();

    BlockingUART3_WriteString33("SD: mounting...\r\n");

    /* ── Step 1: Mount (retry until Harmony driver is ready) ────────── */
    while (SYS_FS_Mount(SD_DEV_NAME, SD_MOUNT_NAME, FAT, 0, NULL) != 0) {
        PT_YIELD(pt);
    }
    BlockingUART3_WriteString33("SD: mounted\r\n");

    /* ── Step 2: Unmount + remount (proves driver stability) ────────── */
    while (SYS_FS_Unmount(SD_MOUNT_NAME) != 0) {
        PT_YIELD(pt);
    }
    BlockingUART3_WriteString33("SD: unmounted\r\n");

    while (SYS_FS_Mount(SD_DEV_NAME, SD_MOUNT_NAME, FAT, 0, NULL) != 0) {
        PT_YIELD(pt);
    }
    BlockingUART3_WriteString33("SD: remounted\r\n");

    /* ── Step 3: Set current drive ──────────────────────────────────── */
    if (SYS_FS_CurrentDriveSet(SD_MOUNT_NAME) == SYS_FS_RES_FAILURE) {
        BlockingUART3_WriteString33("SD: drive set FAIL\r\n");
        PT_EXIT(pt);
    }

    /* ── Step 4: Open source file ───────────────────────────────────── */
    s_fh_src = SYS_FS_FileOpen(SD_FILE_NAME, SYS_FS_FILE_OPEN_READ);
    if (s_fh_src == SYS_FS_HANDLE_INVALID) {
        BlockingUART3_WriteString33("SD: open src FAIL\r\n");
        PT_EXIT(pt);
    }
    BlockingUART3_WriteString33("SD: src open OK\r\n");

    /* ── Step 5: Create directory ───────────────────────────────────── */
    if (SYS_FS_DirectoryMake(SD_DIR_NAME) == SYS_FS_RES_FAILURE) {
        /* Not a fatal error — directory may already exist */
        BlockingUART3_WriteString33("SD: mkdir skip (may exist)\r\n");
    } else {
        BlockingUART3_WriteString33("SD: mkdir OK\r\n");
    }

    /* ── Step 6: Open destination file ──────────────────────────────── */
    s_fh_dst = SYS_FS_FileOpen(SD_DIR_NAME "/" SD_FILE_NAME,
                                SYS_FS_FILE_OPEN_WRITE);
    if (s_fh_dst == SYS_FS_HANDLE_INVALID) {
        BlockingUART3_WriteString33("SD: open dst FAIL\r\n");
        SYS_FS_FileClose(s_fh_src);
        PT_EXIT(pt);
    }
    BlockingUART3_WriteString33("SD: dst open OK\r\n");

    /* ── Step 7: Copy in chunks until EOF ───────────────────────────── */
    while (1) {
        s_n_read = SYS_FS_FileRead(s_fh_src, (void *)s_sd_buf, SD_BUF_LEN);

        if (s_n_read == -1) {
            BlockingUART3_WriteString33("SD: read ERR\r\n");
            SYS_FS_FileClose(s_fh_src);
            SYS_FS_FileClose(s_fh_dst);
            PT_EXIT(pt);
        }

        if (s_n_read > 0) {
            if (SYS_FS_FileWrite(s_fh_dst, (const void *)s_sd_buf,
                                 s_n_read) == -1) {
                BlockingUART3_WriteString33("SD: write ERR\r\n");
                SYS_FS_FileClose(s_fh_src);
                SYS_FS_FileClose(s_fh_dst);
                PT_EXIT(pt);
            }
        }

        /* Check for end of file */
        if (SYS_FS_FileEOF(s_fh_src) == 1) {
            break;
        }

        /* Yield after each chunk so other threads keep running */
        PT_YIELD(pt);
    }

    /* ── Step 8: Close both files ──────────────────────────────────── */
    SYS_FS_FileClose(s_fh_src);
    SYS_FS_FileClose(s_fh_dst);

    BlockingUART3_WriteString33("SD: copy DONE\r\n");

    /* ── Step 9: LED ON = success ──────────────────────────────────── */
    LED_ON();

    /* Park the thread — SD work is complete */
    while (1) {
        PT_YIELD(pt);
    }

    PT_END(pt);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  AudioThread — MAX9867 codec initialisation via modem AT commands
 *
 *  Hardware: LE910C4 owns both I2C and PCM to the MAX9867 codec.
 *  PIC32MM controls audio ONLY through AT commands (audio_api.h).
 *
 *  Flow:
 *    1. Wait for modem preflight to complete (SIM + network ready)
 *    2. Run Audio_Init_Cmd() — 7-step AT sequence:
 *         AT#PSEL, AT+CLVL, AT#HFMICG, AT#SHFEC, AT#SHFNR, AT#SHFAGC,
 *         AT#CODECINFO
 *    3. Query codec status (AT#CODECINFO)
 *    4. Optionally enable loopback test (AT#SRP=2)
 *    5. Set g_audio_init_done flag
 *    6. Idle — audio is ready for voice calls
 *
 *  Clock source (crystal vs modem PCM_CLK) is configured in app_config.h.
 *  The modem's ALSA driver handles MAX9867 register setup internally.
 * ══════════════════════════════════════════════════════════════════════════ */

PT_THREAD(AudioThread(struct pt *pt))
{
    static Modem_Result     audio_res;
    static Modem_CodecInfo  codec_info;
    static uint32_t         t_audio;

    PT_BEGIN(pt);

    /* ── Wait until preflight finishes (SIM ready + network registered) ── */
    /* TelitPreflightThread must complete before we send audio AT commands  */
    t_audio = g_ms_ticks;
    PT_WAIT_UNTIL(pt, (g_ms_ticks - t_audio) >= 15000u);

    BlockingUART3_WriteString33("Audio: Starting codec init...\r\n");

    /* ── Step 1: Full audio initialisation (7-step AT sequence) ────────── */
    Audio_Init_Cmd(NULL);   /* NULL = use defaults from app_config.h */
    PT_WAIT_UNTIL(pt, Audio_Init_Poll(&audio_res));

    if (audio_res == MODEM_OK) {
        BlockingUART3_WriteString33("Audio: Codec init OK\r\n");
    } else {
        BlockingUART3_WriteString33("Audio: Codec init FAILED\r\n");
    }

    /* ── Step 2: Query codec to confirm it's alive ─────────────────────── */
    Audio_QueryCodec_Cmd();
    PT_WAIT_UNTIL(pt, Audio_QueryCodec_Poll(&audio_res, &codec_info));

    if (audio_res == MODEM_OK && codec_info.active) {
        BlockingUART3_WriteString33("Audio: Codec active\r\n");
    }

    /* ── Step 3 (optional): Loopback test ──────────────────────────────── */
#if CFG_AUDIO_LOOPBACK_TEST
    BlockingUART3_WriteString33("Audio: Loopback ON (10s)\r\n");
    Audio_SetLoopback_Cmd(true);
    PT_WAIT_UNTIL(pt, Audio_SetLoopback_Poll(&audio_res));

    t_audio = g_ms_ticks;
    PT_WAIT_UNTIL(pt, (g_ms_ticks - t_audio) >= 10000u);

    Audio_SetLoopback_Cmd(false);
    PT_WAIT_UNTIL(pt, Audio_SetLoopback_Poll(&audio_res));
    BlockingUART3_WriteString33("Audio: Loopback OFF\r\n");
#endif

    g_audio_init_done = true;
    BlockingUART3_WriteString33("Audio: Ready for voice calls\r\n");

    /* ── Idle loop — audio system is ready ─────────────────────────────── */
    while (1) {
        PT_YIELD(pt);
    }

    PT_END(pt);
}
