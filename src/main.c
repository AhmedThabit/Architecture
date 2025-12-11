/*******************************************************************************
 *                       PIC32MM0256GPM064 Pin Assignment                      *
 *                                                                             *
 *   ?????????????????????????????????????????????????????????????     *
 *   ? MCU Pin   ? Signal     ? Function / External Connection     ?           *
 *   ?????????????????????????????????????????????????????????????*
 *   ? RA6       ? U1RX       ? UART1 RX ? ESP32 TX                ? *
 *   ? RC12      ? U1TX       ? UART1 TX ? ESP32 RX                ? *
 *   ? RC6       ? U3TX       ? UART3 TX ? Telit 910C4 RX          ? *
 *   ? RC7       ? U3RX       ? UART3 RX ? Telit 910C4 TX          ? *
 *   ? RD1       ? U3RTS      ? UART3 RTS ? Telit CTS              ? *
 *   ? RD3       ? U3CTS      ? UART3 CTS ? Telit RTS              ? *
 *   ? RA15      ? LED        ? On?board status LED (active high)   ? *
 *   ?????????????????????????????????????????????????????????????     *
 *                                                                             *
 * Note:                                                                       *
 *  ? In MCC Pin Manager, assign RA6?U1RX, RC12?U1TX, RC6?U3TX, RC7?U3RX.    *
 *  ? Configure RD1 and RD3 as digital GPIO for RTS/CTS signals.               *
 *  ? RA15 should be set as a digital output for your status LED.              *
 ******************************************************************************/


/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "peripheral/tmr1/plib_tmr1.h"    // MCC/PLIB?generated Timer1 API
#include "../../pt.h"
#include "lc.h"
#include "lc-switch.h"
#include "../Architecture.X/protothreads.h"
#include "bsp.h"
#include "flash_w25q32.h"
#include "storage.h"




// *****************************************************************************
// Section: UART3 Receive Callback (Telit to ESP32 debug echo)
// *****************************************************************************

void telit_rx_callback(UART_EVENT event, uintptr_t context) {
    if (event == UART_EVENT_READ_THRESHOLD_REACHED) {
        uint8_t buffer[64];
        size_t len = UART3_Read(buffer, sizeof (buffer));
        UART1_Write(buffer, len); // Echo to UART1 (ESP32/debug)
    }
}

void UART1_SendChar(char c) {
    while (U1STAbits.UTXBF);
    U1TXREG = c;
}

void UART1_WriteString(const char *str) {
    while (*str) UART1_SendChar(*str++);
}

////test


#define LED_On() (LATASET = (1UL<<15))
#define LED_Off() (LATACLR = (1UL<<15))

#define LED_LED1_TRIS        TRISAbits.TRISA15
#define LED_LED1_LAT         LATAbits.LATA15


#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0

/////test

#include "flash_w25q32.h"
#include <string.h>
#include <stdio.h>

// declare UART3_WriteString33 prototype or include the header
void UART3_WriteString33(const char *str);  // forward declaration

static void uart3_write_hex(uint8_t byte) {
    const char hex[] = "0123456789ABCDEF";
    char out[3] = { hex[byte >> 4], hex[byte & 0x0F], 0 };
    UART3_WriteString33(out);
}

void test_phonebook_direct(void) {
    UART3_WriteString33("=== test_phonebook_direct() ===\r\n");

    // Optionally call Cfg_Load() once at init, not here if already done
    // Cfg_Load();

    phonebook_set_number(0, "1234");
    phonebook_set_default(0);

    UART3_WriteString33("After phonebook_set_number + set_default\r\n");

    // Now see what send_phonebook_list() sends
    send_phonebook_list();
}


void test_flash_raw(void) {
    //UART3_WriteString33("before Flash init\r\n");
    Flash_Init();
        UART3_WriteString33("after Flash init: \n");
    uint32_t testAddr = 0x00010000; // pick a 4 kB sector boundary
    const char *msg = "Hello, W25Q32!";
    size_t msgLen = strlen(msg) + 1; // include terminating 0

    // 1. Erase the sector before writing (erases 4 kB starting at testAddr)
    Flash_EraseSector(testAddr);
    //UART3_WriteString33("after erase Flash : \n");
    // 2. Write the data; Flash_Write() automatically breaks up page boundaries
    Flash_Write(testAddr, (const uint8_t*) msg, msgLen);

    // 3. Read it back into a buffer
    char buf[32] = {0};
    Flash_Read(testAddr, (uint8_t*) buf, msgLen);
    UART3_WriteString33("msgLen\r\n");
    // 4. Compare and send result via UART3
    if (strcmp(buf, msg) == 0) {
        UART3_WriteString33("Flash test OK\r\n");
    } else {
        UART3_WriteString33("Flash test FAILED\r\nRead back: ");
        for (size_t i = 0; i < msgLen; i++) {
            uart3_write_hex(buf[i]);
            UART3_WriteString33(" ");
        }
        UART3_WriteString33("\r\n");
    }

    // Optional: compute and report CRC32
    uint32_t crc = Flash_CRC32(buf, msgLen);
    char crcMsg[32];
    snprintf(crcMsg, sizeof (crcMsg), "CRC32: %08lX\r\n", (unsigned long) crc);
    UART3_WriteString33(crcMsg);
}




// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {

    //    working led
    //     LED_LED1_TRIS = OUTPUT ;
    //     LED_LED1_LAT = LED_ON ;

    //    INTERRUPT_GlobalInterruptDisable();

    /**
     * @brief Initialize low-level hardware (Layers 1?2).
     * Sets up system clocks, GPIO, UART peripherals, and EVIC interrupts.
     * Call once at startup before higher-layer drivers.
     */
    SYS_Initialize(NULL);

    // main.c
    ESP32_UartInit(); // call after SYS_Initialize
    Flash_Init();
    PhonebookFlash_Init(); 
    
    Cfg_Load(); // load config from external flash

    
    
    //test_phonebook_direct();
    
    
    
    
    // === UART test = One-time UART startup messages  ===
    //    UART1_Write((uint8_t *) "1 Hello ESP32!\r\n", 16);
    //    UART3_Write((uint8_t *) "AT\r\n", 4);
    //    UART1_WriteString("2 Sending NOooSMS...\r\n");
    //    UART1_WriteString("3 TSSSSending SMS...\r\n");

    /**
     * @brief Layer 1 helpers.
     * Provides a 1 ms system tick (Timer1) used by Protothreads.
     */
    BSP_Timer1_Init(); // the 1?ms SysTick for Protothreads

    /* Layer 4 middleware
     * - Initialize UART3 for Telit modem
     * - Attach/register parser callback
     */
    BSP_UART3_Init();


    // somewhere in main loop (once after ESP32_UartInit)
//    static bool test_out_once = false;
//    if (!test_out_once) {
//        const uint8_t ping[] = {0x81, 0x00, 0x01, 0x04, 'P', 'I', 'N', 'G'}; // ROP=0x81, STATUS=OK, TLV tag=1, len=4, "PING"
//        ESP32_SendFrame(ping, sizeof ping);
//        test_out_once = true;
//    }


    /** Layer 3: Protothreads scheduler initialization */
    Protothreads_Init();

    // in main(), after BSP_UART3_Init() and before enabling interrupts:
    UART3_ReadCallbackRegister(telit_rx_callback, 0);
    UART3_ReadThresholdSet(1);
    UART3_ReadNotificationEnable(true, true); // persistent notify

    //UART3_WriteString33("Before storage init\r\n");
    //Storage_Init();
    //UART3_WriteString33("Before flash raw\r\n");
    //test_flash_raw();
    //UART3_WriteString33("after flash raw\r\n");
    //

    (void) __builtin_enable_interrupts();

    //    //===  Register UART3 receive callback for Telit responses === interrupt-based notification =====
    //    UART3_ReadCallbackRegister(telit_rx_callback, 0);
    //    UART3_ReadThresholdSet(1);
    //    UART3_ReadNotificationEnable(true, true); // persistent notification
    //    
    //    BSP_UART3_Init();          // register the Telit UART3 callback

    // -------- BOOT PHASE: run preflight until it returns PT_END --------
    static volatile bool preflight_running = true;
    int f = 0;

    
        while (preflight_running) {
            SYS_Tasks();
    
            // returns 1 while still running, 0 when hits PT_END
            preflight_running = !PT_SCHEDULE(TelitPreflightThread(&ptPreflight));
    
            // keep other threads alive during boot if needed
            PT_SCHEDULE(Esp32Thread(&ptEsp32));
    
        }
    
        while (true) {
            /* Maintain state machines of all polled MPLAB Harmony modules. */
            SYS_Tasks();
    
            // Cooperatively run each Protothread once per loop
            //        SensorThread(&ptSensor);
            //        if(f==0){
            //        UART3_WriteString33("T-est AT\r\n");
            //        f=f+1;
            //        }
        //    TelitThread(&ptTelit);
            Esp32Thread(&ptEsp32);
    
            //        Esp32TxTestThread(&ptEspTxTest);
            //        EthThread(&ptEth);
            //        CliThread(&ptCLI);
    
        }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */

