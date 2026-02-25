/**
 * @file    main.c
 * @brief   Entry point — thin orchestrator.
 *
 * @details All initialisation logic lives in App_Init() (app/app.c).
 *          The cooperative scheduler lives in App_Run() (app/app.c).
 *          main.c is intentionally minimal.
 *
 *  ┌─────────────────────────────────────────────────────────────────────┐
 *  │                  PIC32MM0256GPM064 Pin Assignment                   │
 *  ├──────────┬────────────┬──────────────────────────────────────────── │
 *  │ MCU Pin  │ Signal     │ Function / External Connection             │
 *  ├──────────┼────────────┼──────────────────────────────────────────── │
 *  │ RA6      │ U1RX       │ UART1 RX  ← ESP32 TX                      │
 *  │ RC12     │ U1TX       │ UART1 TX  → ESP32 RX                      │
 *  │ RC6      │ U3TX       │ UART3 TX  → Telit LE910C4 RX              │
 *  │ RC7      │ U3RX       │ UART3 RX  ← Telit LE910C4 TX              │
 *  │ RD1      │ U3RTS      │ UART3 RTS → Telit CTS                     │
 *  │ RD3      │ U3CTS      │ UART3 CTS ← Telit RTS                     │
 *  │ RA15     │ LED        │ On-board status LED (active high)          │
 *  └──────────┴────────────┴──────────────────────────────────────────── │
 */

#include "app/app.h"
#include <stdlib.h>     /* EXIT_FAILURE */

int main(void)
{
    App_Init();
    App_Run();          /* never returns */

    return EXIT_FAILURE;
}
