/**
 * @file    io_monitor.h
 * @brief   HAL — Digital I/O monitoring, output control, and ADC sensors.
 *
 * @details Provides hardware abstraction for:
 *            - 4 digital inputs with NO/NC normalisation and debounce
 *            - 4 digital outputs with set/clear/toggle control
 *            - Moisture sensor ADC reading
 *            - Battery voltage ADC reading
 *            - Mains power present detection
 *
 *          GPIO pin assignments are defined in app_config.h:
 *            CFG_DIN1_PIN .. CFG_DIN4_PIN   (digital inputs)
 *            CFG_DOUT1_PIN .. CFG_DOUT4_PIN (digital outputs)
 *            CFG_MAINS_DET_PIN              (mains power detect)
 */

#ifndef IO_MONITOR_H
#define IO_MONITOR_H

#include <stdint.h>
#include <stdbool.h>

/* ── Initialisation ─────────────────────────────────────────────────────── */

/**
 * @brief  Initialise I/O monitor — configure input/output pins.
 *         Must be called after SYS_Initialize() and BSP_Initialize().
 */
void IO_Init(void);

/* ── Digital inputs ─────────────────────────────────────────────────────── */

/**
 * @brief  Read the 4 digital inputs, applying NO/NC normalisation.
 *         Active input = bit HIGH, regardless of NO/NC configuration.
 * @return Bitmask of inputs (bits 0..3).
 */
uint8_t IO_GetInputsMask(void);

/**
 * @brief  Read a single digital input (normalised).
 * @param  ch  Input channel 0..3
 * @return true if input is active (alarm condition).
 */
bool IO_GetInput(uint8_t ch);

/**
 * @brief  Get the raw (un-normalised) input state before NO/NC inversion.
 * @return Bitmask of raw GPIO levels (bits 0..3).
 */
uint8_t IO_GetInputsRaw(void);

/**
 * @brief  Run debounce logic on all inputs — call every 10 ms from a timer
 *         or protothread.  Updates the internal debounced state.
 */
void IO_DebounceInputs(void);

/**
 * @brief  Get the debounced + normalised input mask.
 * @return Bitmask of debounced active inputs (bits 0..3).
 */
uint8_t IO_GetDebouncedInputs(void);

/* ── Digital outputs ────────────────────────────────────────────────────── */

/**
 * @brief  Read the 4 digital outputs.
 * @return Bitmask of outputs (bits 0..3).
 */
uint8_t IO_GetOutputsMask(void);

/**
 * @brief  Set a specific output ON.
 * @param  ch  Output channel 0..3
 */
void IO_SetOutput(uint8_t ch);

/**
 * @brief  Clear a specific output OFF.
 * @param  ch  Output channel 0..3
 */
void IO_ClearOutput(uint8_t ch);

/**
 * @brief  Write all 4 outputs at once.
 * @param  mask  Bitmask of outputs (bits 0..3): 1=ON, 0=OFF
 */
void IO_WriteOutputs(uint8_t mask);

/* ── Analogue sensors ───────────────────────────────────────────────────── */

/**
 * @brief  Read the moisture sensor as tenths of a percent.
 * @return 0..1000  (e.g. 451 = 45.1%)
 */
uint16_t IO_GetMoistPct10(void);

/**
 * @brief  Read battery voltage in millivolts.
 * @return Battery voltage in mV (e.g. 12600 = 12.6V).
 */
uint16_t IO_GetBatteryMV(void);

/**
 * @brief  Check if mains power is present.
 * @return true if mains power is detected.
 */
bool IO_IsMainsPowerPresent(void);

/* -- Runtime reconfiguration --------------------------------------------- */

/**
 * @brief  Reconfigure a single channel GPIO direction at runtime.
 *         Call after changing IOChannelCfg.function via the mobile app.
 * @param  ch  Channel 0..3
 */
void IO_ReconfigureChannel(uint8_t ch);

/* -- Analogue input readings --------------------------------------------- */

/**
 * @brief  Read the raw 12-bit ADC value for an analogue channel.
 * @param  ch  Channel 0..3 (must be configured as ANA_*)
 * @return 0..4095, or 0 if channel is not analogue.
 */
uint16_t IO_GetAnalogRaw(uint8_t ch);

/**
 * @brief  Read an analogue channel in calibrated engineering units.
 * @param  ch  Channel 0..3
 * @return Calibrated value using the channel's AnalogCfg.
 */
float IO_GetAnalogEng(uint8_t ch);

#endif /* IO_MONITOR_H */
