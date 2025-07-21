#ifndef BSP_H
#define BSP_H

#include <stdint.h>
#include <stdbool.h>


static void Timer1Handler(uint32_t status, uintptr_t context);

void BSP_Timer1_Init(void);

/**
 * @brief Initialize board-specific hardware:
 *        - Configure on?board LEDs as outputs (e.g. RA15)
 *        - Configure user button as input (e.g. RA5)
 *        - Any other GPIOs, pull?ups, default states
 */
void BSP_Initialize(void);

/**
 * @brief Initialize UART3 for Telit 910C4 communication:
 *        - Calls UART3_Initialize()
 *        - Registers the Telit_UART3_Callback
 *        - Enables read notifications
 */
void BSP_UART3_Init(void);

/**
 * @brief Initialize Timer1 to generate a 1?ms tick:
 *        - Calls TMR1_Initialize()
 *        - Registers Timer1Handler callback
 *        - Starts the timer
 * @note  Increments the global msTicks variable in its ISR
 */
void BSP_Timer1_Init(void);

/**
 * @brief Check whether the user has requested entry into the bootloader.
 *        Typically reads a GPIO (e.g. a push?button held low on reset).
 * @return true if bootloader entry is requested
 */
bool BSP_BootloaderRequested(void);

/**
 * @brief Jump to the bootloader?s entry point.
 *        Does not return.
 */
void Bootloader_Jump(void);

/**
 * @brief Millisecond tick counter, incremented by the Timer1 ISR.
 * @see   BSP_Timer1_Init()
 */
extern volatile uint32_t msTicks;

#endif // BSP_H
