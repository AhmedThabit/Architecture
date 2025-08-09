


#include <stdint.h>       // for uint32_t
#include "../../pt.h"
#include "../Architecture.X/protothreads.h"
#include "definitions.h"                // SYS function prototypes





// Millisecond tick, incremented in Timer1 ISR
extern volatile uint32_t msTicks;

// One pt struct per thread
struct pt ptSensor, ptTelit, ptEsp32, ptEth, ptCLI;

void Protothreads_Init(void)
{
    PT_INIT(&ptSensor);
    PT_INIT(&ptTelit);
    PT_INIT(&ptEsp32);
    PT_INIT(&ptEth);
    PT_INIT(&ptCLI);
}

/* ????????? SensorThread ????????? */
PT_THREAD( SensorThread(struct pt *pt) )
{
    static uint32_t t0;
    PT_BEGIN(pt);

    while (1) {
        t0 = msTicks;
//        HAL_ADC_StartConversion();
//        PT_WAIT_UNTIL(pt, HAL_ADC_ConversionComplete());
//        uint16_t val = HAL_ADC_GetResult();
//        processSensor(val);                // your handler
        PT_WAIT_UNTIL(pt, (msTicks - t0) >= 1000);
    }

    PT_END(pt);
}

/* ????????? TelitThread ?????????? */
PT_THREAD( TelitThread(struct pt *pt) )
{
    PT_BEGIN(pt);

    while (1) {
//        PT_WAIT_UNTIL(pt, telitDataReady());
        uint8_t buf[128];
//        int len = UART3_Read(buf, sizeof(buf));
//        handleTelitResponse(buf, len);
    }

    PT_END(pt);
}

/* ????????? Esp32Thread ????????? */
PT_THREAD( Esp32Thread(struct pt *pt) )
{
    PT_BEGIN(pt);

    while (1) {
//        PT_WAIT_UNTIL(pt, esp32DataReady());
        uint8_t buf[128];
        int len = UART1_Read(buf, sizeof(buf));
         // === UART test = One-time UART startup messages  ===
    UART1_Write((uint8_t *) "PT Hello ESP32!\r\n", 14);
    UART3_Write((uint8_t *) "PT AT\r\n", 4);

//        handleEsp32(buf, len);
    }

    PT_END(pt);
}

/* ????????? EthThread ???????????? */
PT_THREAD( EthThread(struct pt *pt) )
{
    PT_BEGIN(pt);

    while (1) {
//        ETH_PeriodicTasks();    // e.g. lwIP timers or packet handling
        PT_YIELD(pt);           // give other threads a chance
    }

    PT_END(pt);
}

/* ????????? CliThread ??????????? */
PT_THREAD( CliThread(struct pt *pt) )
{
    PT_BEGIN(pt);

    while (1) {
//        PT_WAIT_UNTIL(pt, cliInputReady());
//        char cmd[64];
//        cliReadLine(cmd, sizeof(cmd));
//        cliExecute(cmd);
    }

    PT_END(pt);
}
