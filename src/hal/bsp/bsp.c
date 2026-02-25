/**
 * @file    bsp.c
 * @brief   Board Support Package — implementation.
 *
 * @details ONLY hardware-level code.  No #include of telit.h, gsm_service.h,
 *          or any driver/service header.  The modem driver registers its own
 *          UART callback via Modem_Uart_Init() — BSP doesn't need to know.
 */

#include "bsp.h"
#include "peripheral/tmr1/plib_tmr1.h"
#include "definitions.h"

/* ── System tick ────────────────────────────────────────────────────────── */

volatile uint32_t g_ms_ticks = 0;

static void timer1_tick_handler(uint32_t status, uintptr_t context)
{
    (void)status;
    (void)context;
    g_ms_ticks++;
}

void BSP_Timer1_Init(void)
{
    TMR1_Initialize();
    TMR1_CallbackRegister(timer1_tick_handler, (uintptr_t)0);
    TMR1_Start();
}

/* ── LED (RA15, active high) ────────────────────────────────────────────── */

void BSP_Led_On(void)
{
    LATASET = (1UL << CFG_LED_STATUS_BIT);
}

void BSP_Led_Off(void)
{
    LATACLR = (1UL << CFG_LED_STATUS_BIT);
}

void BSP_Led_Toggle(void)
{
    LATAINV = (1UL << CFG_LED_STATUS_BIT);
}

bool BSP_Led_Get(void)
{
    return (PORTA >> CFG_LED_STATUS_BIT) & 0x1;
}

/* ── Board init ─────────────────────────────────────────────────────────── */

void BSP_Initialize(void)
{
    /* Configure RA15 as digital output for status LED */
    ANSELACLR = (1UL << CFG_LED_STATUS_BIT);
    TRISACLR  = (1UL << CFG_LED_STATUS_BIT);
    BSP_Led_Off();
}

bool BSP_BootloaderRequested(void)
{
    /* TODO: read boot button GPIO */
    return false;
}

void BSP_JumpToBootloader(void)
{
    /* TODO: jump to bootloader entry address */
    while (1) { }
}
