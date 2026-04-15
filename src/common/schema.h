/**
 * @file    schema.h
 * @brief   Device configuration data structures.
 *
 * @details Defines the persistent configuration stored in external flash.
 *          Includes phonebook, I/O settings, alarm configuration,
 *          and a CRC for integrity checking.
 *
 *          When you add fields, update Cfg_Defaults() in store.c and
 *          bump the version so old flash data is rejected on first boot.
 */

#ifndef SCHEMA_H
#define SCHEMA_H

#include <stdint.h>
#include <stdbool.h>
#include "config/app_config.h"

/* -- I/O channel function types (matches EDAC 700 + output modes) -------- */

typedef enum {
    IO_FUNC_OFF         = 0,    /**< Disabled / removed for servicing      */
    IO_FUNC_DIG_NO      = 1,    /**< Digital input -- Normally Open        */
    IO_FUNC_DIG_NC      = 2,    /**< Digital input -- Normally Closed      */
    IO_FUNC_ANA_20MA    = 3,    /**< Analogue input -- 4-20mA loop         */
    IO_FUNC_ANA_10V     = 4,    /**< Analogue input -- 0-10V DC            */
    IO_FUNC_ANA_1V      = 5,    /**< Analogue input -- 0-1V DC             */
    IO_FUNC_OUT_OFF     = 6,    /**< Digital output -- OFF on reset        */
    IO_FUNC_OUT_ON      = 7,    /**< Digital output -- ON on reset         */
} IOFunction;

/* -- Analogue trigger types (matches EDAC 700) --------------------------- */

typedef enum {
    ANA_TRIG_WHEN_HIGH  = 0,    /**< Alarm when above set point            */
    ANA_TRIG_WHEN_LOW   = 1,    /**< Alarm when below set point            */
    ANA_TRIG_OUTSIDE    = 2,    /**< Alarm when outside set/reset range    */
    ANA_TRIG_BETWEEN    = 3,    /**< Alarm when between set/reset range    */
} AnaTrigType;

/* -- Analogue configuration (only used when function is ANA_*) ----------- */

typedef struct {
    float    low_raw;           /**< Sensor low value (mA or V)            */
    float    high_raw;          /**< Sensor high value (mA or V)           */
    float    low_eng;           /**< Low engineering units                 */
    float    high_eng;          /**< High engineering units                */
    float    offset;            /**< Calibration offset in eng units       */
    float    set_point;         /**< High alarm threshold (eng units)      */
    float    reset_point;       /**< Low alarm / reset threshold           */
    uint8_t  trig_type;         /**< AnaTrigType enum value                */
    uint8_t  decimal_places;    /**< Display precision (0..4)              */
    uint8_t  reserved[2];       /**< Padding for alignment                 */
} AnalogCfg;

/* -- Per-channel I/O configuration --------------------------------------- */

typedef struct {
    uint8_t  function;          /**< IOFunction enum value                 */
    uint8_t  latch;             /**< 0 = auto-clear, 1 = latched           */
    uint16_t trigger_delay_ms;  /**< Delay before alarm fires (ms)         */
    uint16_t clear_delay_ms;    /**< Delay before alarm clears (ms)        */
    uint8_t  output_map;        /**< Output ch to activate on alarm        *
                                 *   (0..3, 0xFF = none)                   */
    uint8_t  enabled;           /**< 0 = disabled, 1 = enabled             */
    char     label[16];         /**< User-readable label                   */
    char     audio_file[24];    /**< SD card filename for voice message    */
    AnalogCfg analog;           /**< Analogue config (ignored if digital)  */
} IOChannelCfg;

/* Keep backward-compat alias */
typedef IOChannelCfg InputChannelCfg;

/* -- Legacy I/O configuration (kept for backward compat) ----------------- */

typedef struct {
    uint8_t  in_no_nc;          /**< Bit 0..3: 0=NO, 1=NC per input       */
    uint8_t  in_latch;          /**< Bit 0..3: latch flags per input       */
    uint16_t moist_thr;         /**< Moisture threshold (0..1000 tenths%)  */
} InputCfg;

/* -- Other sub-structures ------------------------------------------------ */

typedef struct {
    char    pin[8];             /**< Optional SMS admin PIN                */
    uint8_t reserved;
} GlobalCfg;

typedef struct {
    char    numbers[CFG_PHONEBOOK_MAX_ENTRIES][CFG_PHONEBOOK_NUM_LEN];
    uint8_t default_index;      /**< 0..15 -- default slot for outgoing   */
} PhonebookCfg;

/** Alarm notification preferences. */
typedef struct {
    uint8_t  call_enabled;      /**< 1 = make voice calls on alarm         */
    uint8_t  sms_enabled;       /**< 1 = send SMS on alarm                 */
    uint8_t  max_call_retries;  /**< Max calls before giving up (0..20)    */
    uint16_t call_retry_ms;     /**< Delay between calls (ms)              */
    uint16_t ring_timeout_ms;   /**< Ring time before moving on (ms)       */
    uint16_t battery_low_mv;    /**< Battery low threshold (mV)            */
    uint8_t  reserved[3];       /**< Padding for future use                */
} AlarmNotifyCfg;

/* -- Top-level configuration --------------------------------------------- */

typedef struct {
    GlobalCfg           global;
    InputCfg            io;             /**< Legacy bitmask config         */
    IOChannelCfg        inputs[CFG_IO_COUNT]; /**< Per-channel I/O config  */
    AlarmNotifyCfg      alarm;          /**< Alarm notification prefs      */
    PhonebookCfg        phonebook;
    uint32_t            crc;            /**< CRC32 over all preceding      */
    uint32_t            version;        /**< Config version counter        */
} DeviceCfg;

/* -- Global instance and API --------------------------------------------- */

extern DeviceCfg g_device_cfg;

void Cfg_Load(void);        /**< Load configuration from flash.           */
void Cfg_Save(void);        /**< Save g_device_cfg to flash.              */
void Cfg_Defaults(void);    /**< Reset g_device_cfg to factory defaults.  */
int  Cfg_Validate(void);    /**< Validate CRC.  Returns 0 if valid.      */

#endif /* SCHEMA_H */
