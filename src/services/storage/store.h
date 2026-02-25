#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "common/schema.h"        // for DeviceCfg and phonebook structure

/* --------------------------------------------------------------------------
 * Global configuration structure
 * -------------------------------------------------------------------------- */
extern DeviceCfg g_cfg;

/* --------------------------------------------------------------------------
 * Configuration load/save API
 * -------------------------------------------------------------------------- */

/**
 * @brief Load configuration from W25Q32 flash.
 *        If flash is empty or CRC/magic/version mismatch occurs,
 *        default values are loaded automatically.
 */
void Cfg_Load(void);

/**
 * @brief Save the current g_cfg structure back to flash
 *        (writes header + data + CRC).
 */
void Cfg_Save(void);

/**
 * @brief Reset g_cfg to factory defaults (RAM only).
 *        Version is set to 0x00010100.
 */
void Cfg_Defaults(void);

/**
 * @brief Validate stored config by reloading and checking CRC.
 * @return 0 if OK, non-zero if corrupted.
 */
int Cfg_Validate(void);

/* --------------------------------------------------------------------------
 * Phonebook API (stored inside g_cfg and persisted with Cfg_Save)
 * -------------------------------------------------------------------------- */

/**
 * @brief Get a phone number from slot 0..15.
 * @return pointer to null-terminated string, or NULL if empty.
 */
const char* phonebook_get_number(uint8_t slot);

/**
 * @brief Store a phone number (max 23 digits) into slot 0..15.
 *        Automatically calls Cfg_Save().
 */
void phonebook_set_number(uint8_t slot, const char* num);

/**
 * @brief Delete a phonebook entry (set to empty string).
 *        Automatically calls Cfg_Save().
 */
void phonebook_delete_number(uint8_t slot);

/**
 * @brief Set which slot is the default one used by the system.
 *        Automatically calls Cfg_Save().
 */
void phonebook_set_default(uint8_t slot);

/**
 * @brief Get the default phonebook slot index (0..15).
 */
uint8_t phonebook_get_default(void);

#endif // STORE_H
