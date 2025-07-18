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



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {
    /* Initialize all modules */
    SYS_Initialize(NULL);

    // === UART test = One-time UART startup messages  ===
    UART1_Write((uint8_t *) "Hello ESP32!\r\n", 14);
    UART3_Write((uint8_t *) "AT\r\n", 4);
    
    //===  Register UART3 receive callback for Telit responses === interrupt-based notification =====
    UART3_ReadCallbackRegister(telit_rx_callback, 0);
    UART3_ReadThresholdSet(1);
    UART3_ReadNotificationEnable(true, true); // persistent notification


    while (true) {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks();

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

