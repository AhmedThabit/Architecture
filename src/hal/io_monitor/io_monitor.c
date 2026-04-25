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
#include <xc.h>     /* for ANSELA / ANSELB register access */

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

/* --------------------------------------------------------------------------
 *  ANSEL bit positions for each I/O pin -- needed to switch between
 *  analog and digital mode at runtime. PIC32MM has separate ANSELA/ANSELB
 *  registers; bit number matches the pin number within the port.
 *
 *  RA13 -> ANSELA bit 13   (AN16)
 *  RA12 -> ANSELA bit 12   (AN17)
 *  RA11 -> ANSELA bit 11   (AN18)
 *  RB15 -> ANSELB bit 15   (AN10)
 * -------------------------------------------------------------------------- */

typedef enum { PORT_A = 0, PORT_B = 1 } PortLetter;

typedef struct {
    PortLetter port;
    uint8_t    bit;
} PinAnsel;

static const PinAnsel s_io_ansel[CFG_IO_COUNT] = {
    { PORT_A, 13 },   /* CH1 / RA13 / AN16 */
    { PORT_A, 12 },   /* CH2 / RA12 / AN17 */
    { PORT_A, 11 },   /* CH3 / RA11 / AN18 */
    { PORT_B, 15 },   /* CH4 / RB15 / AN10 */
};

/** Force a pin into digital mode (clear its ANSELx bit). */
static void pin_set_digital(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return;
    uint16_t mask = (uint16_t)(1u << s_io_ansel[ch].bit);
    if (s_io_ansel[ch].port == PORT_A) {
        ANSELACLR = mask;
    } else {
        ANSELBCLR = mask;
    }
}

/** Force a pin into analog mode (set its ANSELx bit). */
static void pin_set_analog(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return;
    uint16_t mask = (uint16_t)(1u << s_io_ansel[ch].bit);
    if (s_io_ansel[ch].port == PORT_A) {
        ANSELASET = mask;
    } else {
        ANSELBSET = mask;
    }
}

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

    /* Configure each channel based on its function in the config.
     * IMPORTANT: the universal I/O pins (RA13/RA12/RA11/RB15) all have
     * ADC capability and default to analog mode. To use them as digital
     * I/O we MUST clear their ANSELx bit -- otherwise GPIO_PinRead always
     * returns 0 and GPIO_PinSet/Clear has no effect on the actual pin. */
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        IO_ReconfigureChannel(i);
    }

    /* MAINS_DET (RB14) and BATTFAIL (RA14) are also ADC-capable pins on
     * the PIC32MM and default to analog. Force them digital too. */
    ANSELBCLR = (1u << 14);   /* RB14 = AN9 */
    ANSELACLR = (1u << 14);   /* RA14 */
    GPIO_PinInputEnable(CFG_MAINS_DET_PIN);
    GPIO_PinInputEnable(CFG_BATTFAIL_PIN);

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
            /* Digital input -- ANSELx bit must be CLEARED for GPIO_PinRead
             * to return real values. This is the bug that was making CH1/CH2/CH3
             * stuck LOW: the pins were still in analog mode. */
            pin_set_digital(ch);
            GPIO_PinInputEnable(s_io_pins[ch]);
            break;
        case IO_FUNC_ANA_20MA:
        case IO_FUNC_ANA_10V:
        case IO_FUNC_ANA_1V:
            /* Analogue input -- ANSELx bit must be SET so the ADC can read */
            pin_set_analog(ch);
            GPIO_PinInputEnable(s_io_pins[ch]);
            break;
        case IO_FUNC_OUT_OFF:
            pin_set_digital(ch);
            GPIO_PinOutputEnable(s_io_pins[ch]);
            GPIO_PinClear(s_io_pins[ch]);
            break;
        case IO_FUNC_OUT_ON:
            pin_set_digital(ch);
            GPIO_PinOutputEnable(s_io_pins[ch]);
            GPIO_PinSet(s_io_pins[ch]);
            break;
        case IO_FUNC_OFF:
        default:
            /* Disabled -- still keep digital so raw-pin diagnostic works */
            pin_set_digital(ch);
            GPIO_PinInputEnable(s_io_pins[ch]);
            break;
    }
}

/* --------------------------------------------------------------------------
 *  Raw pin state -- reads physical pin regardless of channel function.
 *  Useful for debugging/testing to see if a pin is floating or noisy.
 * -------------------------------------------------------------------------- */

bool IO_GetPinRaw(uint8_t ch)
{
    if (ch >= CFG_IO_COUNT) return false;
    return GPIO_PinRead(s_io_pins[ch]);
}

uint8_t IO_GetAllPinsRaw(void)
{
    uint8_t mask = 0;
    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        if (GPIO_PinRead(s_io_pins[i])) mask |= (1u << i);
    }
    return mask;
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
    /* Industrial convention with PCB pull-ups:
     *   NO contact (Normally Open):
     *      - idle:    contact open  -> pin reads HIGH (pulled up) -> NOT alarm
     *      - alarm:   contact closes to GND -> pin reads LOW       -> ALARM
     *      So 'active' = pin LOW. We invert the raw bit.
     *
     *   NC contact (Normally Closed):
     *      - idle:    contact closed to GND -> pin reads LOW       -> NOT alarm
     *      - alarm:   contact opens -> pin reads HIGH (pulled up)  -> ALARM
     *      So 'active' = pin HIGH. Use the raw bit directly.
     */
    uint8_t raw = IO_GetInputsRaw();
    uint8_t result = 0;

    for (uint8_t i = 0; i < CFG_IO_COUNT; i++) {
        uint8_t func = g_device_cfg.inputs[i].function;
        bool pin_high = (raw >> i) & 0x01;
        bool active = false;

        if (func == IO_FUNC_DIG_NO) {
            /* NO: alarm when pin goes LOW (contact closes to GND) */
            active = !pin_high;
        } else if (func == IO_FUNC_DIG_NC) {
            /* NC: alarm when pin goes HIGH (contact opens, pull-up wins) */
            active = pin_high;
        }

        if (active) result |= (1u << i);
    }
    return result & 0x0F;
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
    /* Battery voltage divider not yet implemented -- but BATTFAIL pin
     * does indicate low battery: HIGH = OK, LOW = battery fault.
     * For now, return a synthetic value based on the digital fail signal. */
    if (GPIO_PinRead(CFG_BATTFAIL_PIN)) {
        return 12600;   /* battery OK -- placeholder voltage */
    } else {
        return 9800;    /* battery FAIL -- below threshold */
    }
}

bool IO_IsBatteryOK(void)
{
    return GPIO_PinRead(CFG_BATTFAIL_PIN);
}

bool IO_IsMainsPowerPresent(void)
{
    return GPIO_PinRead(CFG_MAINS_DET_PIN);
}
