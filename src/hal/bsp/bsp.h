/**
 * @file    bsp.h
 * @brief   Board Support Package — PIC32MM USB Curiosity board.
 *
 * @details Hardware-only abstraction.  Knows about GPIO pins, LEDs,
 *          timers, and UART peripherals — but NOT about Telit, ESP32,
 *          or any application-level protocol.
 */

#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "config/app_config.h"

/* ── System tick (1 ms, driven by Timer1 ISR) ───────────────────────────── */

/** Millisecond counter — incremented by the Timer1 ISR. */
extern volatile uint32_t g_ms_ticks;

/**
 * @brief  Initialise Timer1 for a 1 ms periodic tick.
 *         Registers the ISR, starts the timer.
 */
void BSP_Timer1_Init(void);

/* ── Board identification ───────────────────────────────────────────────── */

#define BSP_NAME    CFG_BOARD_NAME

/* ── LED control (RA15 — active high) ───────────────────────────────────── */

void BSP_Led_On(void);
void BSP_Led_Off(void);
void BSP_Led_Toggle(void);
bool BSP_Led_Get(void);

/* ── Convenience macros (backward-compatible) ───────────────────────────── */

#define LED1_On()       BSP_Led_On()
#define LED1_Off()      BSP_Led_Off()
#define LED1_Toggle()   BSP_Led_Toggle()
#define LED1_Get()      BSP_Led_Get()

/* ── Boot ───────────────────────────────────────────────────────────────── */

/**
 * @brief  Initialise board-level GPIO (LEDs, buttons, pull-ups).
 *         Called by App_Init() before any driver or service.
 */
void BSP_Initialize(void);

/**
 * @brief  Check whether the user is requesting bootloader entry.
 * @return true if bootloader entry button is held.
 */
bool BSP_BootloaderRequested(void);

/**
 * @brief  Jump to the bootloader entry point (does not return).
 */
void BSP_JumpToBootloader(void);

#endif /* BSP_H */
