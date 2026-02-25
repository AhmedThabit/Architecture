/**
 * @file    schema.h
 * @brief   Device configuration data structures.
 *
 * @details Defines the persistent configuration stored in external flash.
 *          Includes phonebook, I/O settings, and a CRC for integrity checking.
 */

#ifndef SCHEMA_H
#define SCHEMA_H

#include <stdint.h>
#include "config/app_config.h"

/* ── Configuration sub-structures ───────────────────────────────────────── */

typedef struct {
    char    pin[8];             /**< Optional SMS admin PIN                */
    uint8_t reserved;
} GlobalCfg;

typedef struct {
    uint8_t  in_no_nc;          /**< Bit 0..3: 0=NO, 1=NC per input      */
    uint8_t  in_latch;          /**< Bit 0..3: latch flags per input      */
    uint16_t moist_thr;         /**< Moisture threshold (0..1000 tenths%) */
} InputCfg;

typedef struct {
    char    numbers[CFG_PHONEBOOK_MAX_ENTRIES][CFG_PHONEBOOK_NUM_LEN];
    uint8_t default_index;      /**< 0..15 — default slot for outgoing    */
} PhonebookCfg;

/* ── Top-level configuration ────────────────────────────────────────────── */

typedef struct {
    GlobalCfg       global;
    InputCfg        io;
    PhonebookCfg    phonebook;
    uint32_t        crc;        /**< CRC32 over all preceding fields      */
    uint32_t        version;    /**< Configuration version counter        */
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
