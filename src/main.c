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






// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {

//    working led
//     LED_LED1_TRIS = OUTPUT ;
//     LED_LED1_LAT = LED_ON ;
    
    //Notworked 
    //UART1_Write((uint8_t *) "Hello SSSSS!\r\n", 14);
    
//    INTERRUPT_GlobalInterruptDisable();
    /* ????????? Layer?1+2 ????????
       ? clocks, GPIO, UARTs, EVIC ?
       ?????????????????????????? */
    /* Initialize all modules */
    SYS_Initialize(NULL);
    
    
    
    // === UART test = One-time UART startup messages  ===
    UART1_Write((uint8_t *) "Hello ESP32!\r\n", 14);
    UART3_Write((uint8_t *) "AT\r\n", 4);
    UART1_WriteString("Sending NOooSMS...\r\n");

    UART1_WriteString("TSSSSending SMS...\r\n");
    /* ?????????? Layer?1 Helpers ??????????
       ? 1?ms SysTick for Protothreads   ?
       ???????????????????????????????????? */
    
//    BSP_Timer1_Init();        // the 1?ms SysTick for Protothreads
    
     /* ?????????? Layer?4 Middleware ???????
       ? Telit UART3 setup & parser hook ?
       ???????????????????????????????????? */
//    
//    
//    BSP_UART3_Init();
//    Telit_Init();
//    Protothreads_Init();       // Layer?3 scheduler
    
//    INTERRUPT_GlobalInterruptEnable();


    // === UART test = One-time UART startup messages  ===
    UART1_Write((uint8_t *) "Hello ESP32!\r\n", 14);
    UART3_Write((uint8_t *) "AT\r\n", 4);

//    //===  Register UART3 receive callback for Telit responses === interrupt-based notification =====
//    UART3_ReadCallbackRegister(telit_rx_callback, 0);
//    UART3_ReadThresholdSet(1);
//    UART3_ReadNotificationEnable(true, true); // persistent notification
//    
//    BSP_UART3_Init();          // register the Telit UART3 callback


    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
         UART1_WriteString("Sending NOooSMS...\r\n");
        
        SYS_Tasks();
        
        UART1_WriteString("Sending NOooSMS...\r\n");
         
        // Run each Protothread once per loop
//        SensorThread(&ptSensor);
//        TelitThread(&ptTelit);
//        Esp32Thread(&ptEsp32);
//        EthThread(&ptEth);
//        CliThread(&ptCLI);
        
        

        // === (Optional) Polling-based UART3 RX ? avoid redundancy ===
        // This can conflict with interrupt-based read.
        // Keep only for early debug/testing, then remove or disable.
        /*
        uint8_t rxData[128];
        size_t rxLen = UART3_Read(rxData, sizeof(rxData));
        if (rxLen > 0)
        {
            UART1_Write(rxData, rxLen);
         //
        }
         */

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */

