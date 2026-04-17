/**
 * @file    io_monitor.c
 * @brief   HAL -- Universal I/O monitoring, output control, ADC sensors.
 *
 * @details Manages 4 universal I/O channels that can each be configured as
 *          digital input (NO/NC), analogue input (1V/10V/20mA), or digital
 *          output. Pin function is determined by the IOChannelCfg.function
 *          field in the device configuration.
 *
 *          Pin assignments come from app_config.h (CFG_IOx_PIN, CFG_IOx_ADC_CH).
 */

#include "io_monitor.h"
#include "config/app_config.h"
#include "common/schema.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/peripheral/adc/plib_adc.h"

/* --------------------------------------------------------------------------
 *  Pin tables -- built from app_config.h macros
 * -------------------------------------------------------------------------- */

static const GPIO_PIN s_io_pins[CFG_IO_COUNT] = {
    CFG_IO1_PIN,
    CFG_IO2_PIN,
    CFG_IO3_PIN,
    CFG_IO4_PIN,
};

static const ADC_INPUT_POSITIVE s_adc_channels[CFG_IO_COUNT] = {
    CFG_IO1_ADC_CH,
    CFG_IO2_ADC_CH,
    CFG_IO3_ADC_CH,
    CFG_IO4_ADC_CH,
};

/* Legacy aliases for code that still uses s_input_pins / s_output_pins */
#define s_input_pins  s_io_pins
#define s_output_pins s_io_pins

/* --------------------------------------------------------------------------
 *  Debounce state -- 3-sample majority filter, called every 10 ms
 * -------------------------------------------------------------------------- */

#define DEBOUNCE_SAMPLES  3

static uint8_t s_debounce_history[DEBOUNCE_SAMPLES];
static uint8_t s_debounce_idx = 0;
static uint8_t s_debounced_mask = 0;

/* --------------------------------------------------------------------------
 *  ADC helper -- blocking single-channel read with timeout (safe)
 * -------------------------------------------------------------------------- */

static uint16_t adc_read_channel(ADC_INPUT_POSITIVE ch)
{
    ADC_InputSelect(ch);
    ADC_SamplingStart();

    /* Brief delay for acquisition (Tad * sample time) */
    for (volatile int i = 0; i < 100; i++) {}

    ADC_ConversionStart();

    /* Wait for conversion complete WITH TIMEOUT to prevent system hang */
    volatile uint32_t timeout = 100000;
    while (!ADC_ResultIsReady() && timeout > 0) {
        timeout--;
    }
    if (timeout == 0) {
        /* ADC failed -- return 0 instead of hanging the system */
        return 0;
    }

    return (uint16_t)(ADC_ResultGet(ADC_RESULT_BUFFER_0) & 0x0FFF);
}

/* --------------------------------------------------------------------------
 *  Initialisation -- configure each channel based on its function type
 * -------------------------------------------------------------------------- */

void IO_Init(void)
{
    /* Initialise ADC peripheral */
    ADC_Initialize();
    ADC_Enable();

    /* Configure each channel based on its function in the config */
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        uint8_t func = g_device_cfg.inputs[i].function;

        switch (func) {
            case IO_FUNC_DIG_NO:
            case IO_FUNC_DIG_NC:
                /* Digital input mode */
                GPIO_PinInputEnable(s_io_pins[i]);
                break;

            case IO_FUNC_ANA_20MA:
            case IO_FUNC_ANA_10V:
            case IO_FUNC_ANA_1V:
                /* Analogue input mode -- pin stays as analog (MCC configured) */
                GPIO_PinInputEnable(s_io_pins[i]);
                break;

            case IO_FUNC_OUT_OFF:
                /* Digital output, start OFF */
                GPIO_PinOutputEnable(s_io_pins[i]);
                GPIO_PinClear(s_io_pins[i]);
                break;

            case IO_FUNC_OUT_ON:
                /* Digital output, start ON */
                GPIO_PinOutputEnable(s_io_pins[i]);
                GPIO_PinSet(s_io_pins[i]);
                break;

            case IO_FUNC_OFF:
            default:
                /* Disabled -- set as input (high-Z) */
                GPIO_PinInputEnable(s_io_pins[i]);
                break;
        }
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

/* --------------------------------------------------------------------------
 *  Reconfigure a single channel at runtime (called after app changes config)
 * -------------------------------------------------------------------------- */

void IO_ReconfigureChannel(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return;
    uint8_t func = g_device_cfg.inputs[ch].function;

    switch (func) {
        case IO_FUNC_DIG_NO:
        case IO_FUNC_DIG_NC:
        case IO_FUNC_ANA_20MA:
        case IO_FUNC_ANA_10V:
        case IO_FUNC_ANA_1V:
            GPIO_PinInputEnable(s_io_pins[ch]);
            break;
        case IO_FUNC_OUT_OFF:
            GPIO_PinOutputEnable(s_io_pins[ch]);
            GPIO_PinClear(s_io_pins[ch]);
            break;
        case IO_FUNC_OUT_ON:
            GPIO_PinOutputEnable(s_io_pins[ch]);
            GPIO_PinSet(s_io_pins[ch]);
            break;
        default:
            GPIO_PinInputEnable(s_io_pins[ch]);
            break;
    }
}

/* --------------------------------------------------------------------------
 *  Digital Inputs -- only reads channels configured as digital inputs
 * -------------------------------------------------------------------------- */

uint8_t IO_GetInputsRaw(void)
{
    uint8_t raw = 0;
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        uint8_t func = g_device_cfg.inputs[i].function;
        if (func == IO_FUNC_DIG_NO || func == IO_FUNC_DIG_NC) {
            if (GPIO_PinRead(s_io_pins[i])) {
                raw |= (1u << i);
            }
        }
    }
    return raw;
}

uint8_t IO_GetInputsMask(void)
{
    uint8_t raw = IO_GetInputsRaw();
    /* Build NC inversion mask from per-channel function type */
    uint8_t inv = 0;
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        if (g_device_cfg.inputs[i].function == IO_FUNC_DIG_NC) {
            inv |= (1u << i);
        }
    }
    return (raw ^ inv) & 0x0F;
}

bool IO_GetInput(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return false;
    return (IO_GetInputsMask() >> ch) & 0x01;
}

void IO_DebounceInputs(void)
{
    s_debounce_history[s_debounce_idx] = IO_GetInputsMask();
    s_debounce_idx = (s_debounce_idx + 1) % DEBOUNCE_SAMPLES;

    uint8_t result = 0;
    for (uint8_t bit = 0; bit < CFG_IO_COUNT; bit++) {
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

/* --------------------------------------------------------------------------
 *  Analogue Inputs -- read raw ADC value for a channel
 * -------------------------------------------------------------------------- */

uint16_t IO_GetAnalogRaw(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return 0;
    uint8_t func = g_device_cfg.inputs[ch].function;
    if (func != IO_FUNC_ANA_20MA && func != IO_FUNC_ANA_10V && func != IO_FUNC_ANA_1V) {
        return 0;
    }
    return adc_read_channel(s_adc_channels[ch]);
}

/**
 * @brief  Convert raw ADC to engineering units using channel calibration.
 * @param  ch  Channel 0..3
 * @return Value in engineering units (e.g. degrees, percent, PSI)
 */
float IO_GetAnalogEng(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return 0.0f;

    const IOChannelCfg *cfg = &g_device_cfg.inputs[ch];
    const AnalogCfg *acfg = &cfg->analog;
    uint8_t func = cfg->function;

    /* Only read ADC if channel is configured as analogue.
     * This prevents accidentally reading ADC on digital/output channels. */
    if (func != IO_FUNC_ANA_1V && func != IO_FUNC_ANA_10V && func != IO_FUNC_ANA_20MA) {
        return 0.0f;
    }

    /* Determine the ADC full-scale for the input type */
    float adc_raw = (float)adc_read_channel(s_adc_channels[ch]);
    float sensor_value = 0.0f;

    /* Map ADC count (0-4095) to sensor range */
    float adc_max = 4095.0f;

    switch (func) {
        case IO_FUNC_ANA_1V:
            sensor_value = (adc_raw / adc_max) * 1.0f;
            break;
        case IO_FUNC_ANA_10V:
            sensor_value = (adc_raw / adc_max) * 10.0f;
            break;
        case IO_FUNC_ANA_20MA:
            sensor_value = (adc_raw / adc_max) * 20.0f;
            break;
        default:
            return 0.0f;
    }

    /* Apply calibration: eng = slope * sensor + offset */
    float range_raw = acfg->high_raw - acfg->low_raw;
    float range_eng = acfg->high_eng - acfg->low_eng;

    /* Guard against bad calibration values (would cause divide-by-zero) */
    if (range_raw < 0.0001f && range_raw > -0.0001f) return acfg->offset;

    float slope = range_eng / range_raw;
    float eng = acfg->low_eng + slope * (sensor_value - acfg->low_raw) + acfg->offset;

    return eng;
}

/* --------------------------------------------------------------------------
 *  Digital Outputs -- only operates on channels configured as outputs
 * -------------------------------------------------------------------------- */

uint8_t IO_GetOutputsMask(void)
{
    uint8_t mask = 0;
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        uint8_t func = g_device_cfg.inputs[i].function;
        if (func == IO_FUNC_OUT_OFF || func == IO_FUNC_OUT_ON) {
            if (GPIO_PinLatchRead(s_io_pins[i])) {
                mask |= (1u << i);
            }
        }
    }
    return mask;
}

void IO_SetOutput(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return;
    uint8_t func = g_device_cfg.inputs[ch].function;
    if (func == IO_FUNC_OUT_OFF || func == IO_FUNC_OUT_ON) {
        GPIO_PinSet(s_io_pins[ch]);
    }
}

void IO_ClearOutput(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return;
    uint8_t func = g_device_cfg.inputs[ch].function;
    if (func == IO_FUNC_OUT_OFF || func == IO_FUNC_OUT_ON) {
        GPIO_PinClear(s_io_pins[ch]);
    }
}

void IO_WriteOutputs(uint8_t mask)
{
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        uint8_t func = g_device_cfg.inputs[i].function;
        if (func == IO_FUNC_OUT_OFF || func == IO_FUNC_OUT_ON) {
            if (mask & (1u << i)) {
                GPIO_PinSet(s_io_pins[i]);
            } else {
                GPIO_PinClear(s_io_pins[i]);
            }
        }
    }
}

/* --------------------------------------------------------------------------
 *  System sensors (battery, mains, moisture)
 * -------------------------------------------------------------------------- */

uint16_t IO_GetMoistPct10(void)
{
    /* TODO: assign a dedicated ADC channel for moisture if needed */
    return 0;
}

uint16_t IO_GetBatteryMV(void)
{
    /* TODO: assign a dedicated ADC channel for battery voltage divider */
    return 12600;   /* placeholder */
}

bool IO_IsMainsPowerPresent(void)
{
    return GPIO_PinRead(CFG_MAINS_DET_PIN);
}
