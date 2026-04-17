/**
 * @file    debug.h
 * @brief   Debug output macros -- UART3 serial and/or BLE log channel.
 *
 * @details Three macros are provided:
 *            DBG_UART(fmt, ...)  -- print to UART3 only (when CFG_DBG_UART3=1)
 *            DBG_BLE(fmt, ...)   -- send to Android Debug Log screen via BLE
 *            DBG(fmt, ...)       -- send to BOTH UART3 and BLE
 *
 *          Use DBG_BLE for messages that should appear in the app's debug log.
 *          Use DBG_UART for messages that are only useful on a serial terminal.
 *          Use DBG for important events you want to see in both places.
 */

#ifndef COMMON_DEBUG_H
#define COMMON_DEBUG_H

#include <stdio.h>
#include <string.h>
#include "config/app_config.h"

/* Forward declarations (implemented in esp32_proto.c / protothreads.c) */
void Debug_SendBle(const char *msg);
void BlockingUART3_WriteString33(const char *s);

/* -------------------------------------------------------------------------
 *  DBG_UART -- print to UART3 only, compiled out when CFG_DBG_UART3 = 0
 * ------------------------------------------------------------------------- */
#if CFG_DBG_UART3
#define DBG_UART(fmt, ...) do {                                 \
    char _dbgbuf[96];                                           \
    snprintf(_dbgbuf, sizeof(_dbgbuf), fmt, ##__VA_ARGS__);     \
    BlockingUART3_WriteString33(_dbgbuf);                       \
} while (0)
#else
#define DBG_UART(fmt, ...) do { (void)(fmt); } while (0)
#endif

/* -------------------------------------------------------------------------
 *  DBG_BLE -- send a TLV 0x7F log message to the Android Debug Log screen.
 *  Always active (the app decides whether to show it).
 * ------------------------------------------------------------------------- */
#define DBG_BLE(fmt, ...) do {                                  \
    char _blebuf[96];                                           \
    snprintf(_blebuf, sizeof(_blebuf), fmt, ##__VA_ARGS__);     \
    Debug_SendBle(_blebuf);                                     \
} while (0)

/* -------------------------------------------------------------------------
 *  DBG -- send to BOTH UART3 and BLE (use for important events).
 * ------------------------------------------------------------------------- */
#define DBG(fmt, ...) do {                                      \
    char _bothbuf[96];                                          \
    snprintf(_bothbuf, sizeof(_bothbuf), fmt, ##__VA_ARGS__);   \
    Debug_SendBle(_bothbuf);                                    \
    DBG_UART("%s", _bothbuf);                                   \
} while (0)

#endif /* COMMON_DEBUG_H */
