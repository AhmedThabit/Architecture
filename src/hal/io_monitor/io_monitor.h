/**
 * @file    io_monitor.h
 * @brief   HAL — Digital I/O and moisture ADC monitor.
 */

#ifndef IO_MONITOR_H
#define IO_MONITOR_H

#include <stdint.h>

/**
 * @brief  Read the 4 digital inputs, applying NO/NC normalisation.
 * @return Bitmask of inputs (bits 0..3).
 */
uint8_t IO_GetInputsMask(void);

/**
 * @brief  Read the 4 digital outputs.
 * @return Bitmask of outputs (bits 0..3).
 */
uint8_t IO_GetOutputsMask(void);

/**
 * @brief  Read the moisture sensor as tenths of a percent.
 * @return 0..1000  (e.g. 451 = 45.1%)
 */
uint16_t IO_GetMoistPct10(void);

#endif /* IO_MONITOR_H */
