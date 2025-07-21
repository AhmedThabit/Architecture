#include "telit.h"
#include "peripheral/uart/plib_uart3.h"
#include "peripheral/tmr1/plib_tmr1.h"    // MCC/PLIB?generated Timer1 API



// 1) Global tick counter for Protothreads
volatile uint32_t msTicks = 0;


// 2) Your Timer1 ?ISR? callback: just bump msTicks
static void Timer1Handler(uint32_t status, uintptr_t context)
{
    (void)status;   // unused
    (void)context;  // unused
    msTicks++;
}


// 3) Initialize & start Timer1 with your callback
void BSP_Timer1_Init(void)
{
    // plib_tmr1.c?s init already set TCKPS=1:8 and PR1=5999 ? 1?ms @ PBCLK=48?MHz
    TMR1_Initialize();

    // tell PLIB to call us on every interrupt
    TMR1_CallbackRegister(Timer1Handler, (uintptr_t)NULL);

    // fire up the timer
    TMR1_Start();
}

// This runs in interrupt-context every time UART3 has data
static void Telit_UART3_Callback(UART_EVENT event, uintptr_t context)
{
    uint8_t b;
    // Read all available bytes, feeding them into your parser
    while (UART3_Read(&b, 1) == 1) {
        handleTelitResponse(&b, 1);
    }
}

void BSP_UART3_Init(void)
{
    UART3_Initialize();
    UART3_ReadCallbackRegister(Telit_UART3_Callback, 0);
    UART3_ReadThresholdSet(1);
    UART3_ReadNotificationEnable(true, true);
}
