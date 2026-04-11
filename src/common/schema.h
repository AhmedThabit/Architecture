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

/* ── Per-input alarm configuration ──────────────────────────────────────── */

/** Per-input configuration — one entry per digital input channel. */
typedef struct {
    uint8_t  polarity;          /**< 0 = Normally Open, 1 = Normally Closed*/
    uint8_t  latch;             /**< 0 = auto-clear, 1 = latched           */
    uint16_t trigger_delay_ms;  /**< Delay before alarm fires (ms)         */
    uint16_t clear_delay_ms;    /**< Delay before alarm clears (ms)        */
    uint8_t  output_map;        /**< Output to activate on alarm (0..3,    *
                                 *   0xFF = none)                          */
    uint8_t  enabled;           /**< 0 = disabled, 1 = enabled             */
    char     label[16];         /**< User-readable label, e.g. "Front Door"*/
    char     audio_file[24];    /**< SD card filename for voice message    */
} InputChannelCfg;

/* ── Legacy I/O configuration (kept for backward compat) ────────────────── */

typedef struct {
    uint8_t  in_no_nc;          /**< Bit 0..3: 0=NO, 1=NC per input       */
    uint8_t  in_latch;          /**< Bit 0..3: latch flags per input       */
    uint16_t moist_thr;         /**< Moisture threshold (0..1000 tenths%)  */
} InputCfg;

/* ── Other sub-structures ───────────────────────────────────────────────── */

typedef struct {
    char    pin[8];             /**< Optional SMS admin PIN                */
    uint8_t reserved;
} GlobalCfg;

typedef struct {
    char    numbers[CFG_PHONEBOOK_MAX_ENTRIES][CFG_PHONEBOOK_NUM_LEN];
    uint8_t default_index;      /**< 0..15 — default slot for outgoing    */
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

/* ── Top-level configuration ────────────────────────────────────────────── */

typedef struct {
    GlobalCfg           global;
    InputCfg            io;             /**< Legacy bitmask config         */
    InputChannelCfg     inputs[CFG_DIN_COUNT]; /**< Per-input config       */
    AlarmNotifyCfg      alarm;          /**< Alarm notification prefs      */
    PhonebookCfg        phonebook;
    uint32_t            crc;            /**< CRC32 over all preceding      */
    uint32_t            version;        /**< Config version counter        */
} DeviceCfg;

/* ── Global instance and API ────────────────────────────────────────────── */

extern DeviceCfg g_device_cfg;

/** Load configuration from flash into g_device_cfg. */
void Cfg_Load(void);

/** Save g_device_cfg to flash. */
void Cfg_Save(void);

/** Reset g_device_cfg to factory defaults. */
void Cfg_Defaults(void);

/** Validate CRC of the current configuration.  Returns 0 if valid. */
int  Cfg_Validate(void);

#endif /* SCHEMA_H */
