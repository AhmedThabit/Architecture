/**
 * @file    io_monitor.c
 * @brief   HAL — Digital I/O monitoring, output control, ADC sensors.
 *
 * @details Reads GPIO pins for 4 digital inputs and controls 4 digital
 *          outputs.  Inputs are debounced (3-sample majority) and
 *          normalised using the NO/NC bitmask from configuration.
 *
 *          Pin assignments come from app_config.h (CFG_DINx_PIN,
 *          CFG_DOUTx_PIN).  These map to Harmony GPIO_PIN_Rxy values.
 *
 *          Battery and mains monitoring use ADC and GPIO respectively.
 *          ADC functions are placeholder until the ADC peripheral is
 *          configured in MCC.
 */

#include "io_monitor.h"
#include "config/app_config.h"
#include "common/schema.h"
#include "config/default/peripheral/gpio/plib_gpio.h"

/* ══════════════════════════════════════════════════════════════════════════
 *  Pin tables — built from app_config.h macros
 * ══════════════════════════════════════════════════════════════════════════ */

static const GPIO_PIN s_input_pins[4] = {
    CFG_DIN1_PIN,
    CFG_DIN2_PIN,
    CFG_DIN3_PIN,
    CFG_DIN4_PIN,
};

static const GPIO_PIN s_output_pins[4] = {
    CFG_DOUT1_PIN,
    CFG_DOUT2_PIN,
    CFG_DOUT3_PIN,
    CFG_DOUT4_PIN,
};

/* ══════════════════════════════════════════════════════════════════════════
 *  Debounce state — 3-sample majority filter, called every 10 ms
 * ══════════════════════════════════════════════════════════════════════════ */

#define DEBOUNCE_SAMPLES  3

static uint8_t s_debounce_history[DEBOUNCE_SAMPLES];
static uint8_t s_debounce_idx = 0;
static uint8_t s_debounced_mask = 0;

/* ══════════════════════════════════════════════════════════════════════════
 *  Initialisation
 * ══════════════════════════════════════════════════════════════════════════ */

void IO_Init(void)
{
    /* Configure input pins as digital inputs */
    for (uint8_t i = 0; i < 4; i++) {
        GPIO_PinInputEnable(s_input_pins[i]);
    }

    /* Configure output pins as digital outputs, start OFF */
    for (uint8_t i = 0; i < 4; i++) {
        GPIO_PinOutputEnable(s_output_pins[i]);
        GPIO_PinClear(s_output_pins[i]);
    }

    /* Configure mains detect pin as input */
    GPIO_PinInputEnable(CFG_MAINS_DET_PIN);

    /* Clear debounce history */
    for (uint8_t i = 0; i < DEBOUNCE_SAMPLES; i++) {
        s_debounce_history[i] = 0;
    }
    s_debounce_idx = 0;
    s_debounced_mask = 0;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Digital Inputs
 * ══════════════════════════════════════════════════════════════════════════ */

uint8_t IO_GetInputsRaw(void)
{
    uint8_t raw = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (GPIO_PinRead(s_input_pins[i])) {
            raw |= (1u << i);
        }
    }
    return raw;
}

uint8_t IO_GetInputsMask(void)
{
    uint8_t raw = IO_GetInputsRaw();
    /* XOR with NO/NC config: bit=1 means NC (invert that input) */
    uint8_t inv = g_device_cfg.io.in_no_nc;
    return (raw ^ inv) & 0x0F;
}

bool IO_GetInput(uint8_t ch)
{
    if (ch > 3) return false;
    return (IO_GetInputsMask() >> ch) & 0x01;
}

void IO_DebounceInputs(void)
{
    /* Store current normalised sample */
    s_debounce_history[s_debounce_idx] = IO_GetInputsMask();
    s_debounce_idx = (s_debounce_idx + 1) % DEBOUNCE_SAMPLES;

    /* Majority vote: bit is set if 2+ of 3 samples agree */
    uint8_t result = 0;
    for (uint8_t bit = 0; bit < 4; bit++) {
        uint8_t count = 0;
        for (uint8_t s = 0; s < DEBOUNCE_SAMPLES; s++) {
            if (s_debounce_history[s] & (1u << bit)) count++;
        }
        if (count >= 2) result |= (1u << bit);
    }
    s_debounced_mask = result;
}

uint8_t IO_GetDebouncedInputs(void)
{
    return s_debounced_mask;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Digital Outputs
 * ══════════════════════════════════════════════════════════════════════════ */

uint8_t IO_GetOutputsMask(void)
{
    uint8_t mask = 0;
    for (uint8_t i = 0; i < 4; i++) {
        if (GPIO_PinLatchRead(s_output_pins[i])) {
            mask |= (1u << i);
        }
    }
    return mask;
}

void IO_SetOutput(uint8_t ch)
{
    if (ch > 3) return;
    GPIO_PinSet(s_output_pins[ch]);
}

void IO_ClearOutput(uint8_t ch)
{
    if (ch > 3) return;
    GPIO_PinClear(s_output_pins[ch]);
}

void IO_WriteOutputs(uint8_t mask)
{
    for (uint8_t i = 0; i < 4; i++) {
        if (mask & (1u << i)) {
            GPIO_PinSet(s_output_pins[i]);
        } else {
            GPIO_PinClear(s_output_pins[i]);
        }
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Analogue Sensors
 *
 *  NOTE: ADC peripheral must be configured in MCC (Harmony Configurator)
 *        for the battery and moisture channels.  These functions use
 *        placeholder values until the ADC PLIB is available.
 * ══════════════════════════════════════════════════════════════════════════ */

uint16_t IO_GetMoistPct10(void)
{
    /* TODO: Read actual ADC channel for moisture sensor.
     *
     * Example with Harmony ADC PLIB:
     *   ADC1_ChannelSelect(CFG_ADC_MOIST_CH);
     *   ADC1_Start();
     *   while (!ADC1_IsReady()) {}
     *   uint16_t adc = ADC1_ResultGet();
     *   return (uint16_t)((adc * 1000UL) / 4095UL);
     */
    return 0;   /* 0.0% — placeholder */
}

uint16_t IO_GetBatteryMV(void)
{
    /* TODO: Read actual ADC channel for battery voltage divider.
     *
     * Assuming a voltage divider that maps 0–15V → 0–3.3V:
     *   uint16_t adc = ADC1_ResultGet();
     *   float volts = (adc / 4095.0f) * 3.3f * (15.0f / 3.3f);
     *   return (uint16_t)(volts * 1000.0f);
     *
     * Or integer math:
     *   return (uint16_t)((uint32_t)adc * 15000UL / 4095UL);
     */
    return 12600;   /* 12.6V — placeholder (fully charged SLA) */
}

bool IO_IsMainsPowerPresent(void)
{
    /* Read mains detection pin — active HIGH when mains present */
    return GPIO_PinRead(CFG_MAINS_DET_PIN);
}
