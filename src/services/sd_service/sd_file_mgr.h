/**
 * @file    sd_file_mgr.h
 * @brief   SD card file manager — deferred operations via request queue.
 *
 * @details File operations cannot run inside Esp_HandleFrame() because
 *          SYS_FS needs SYS_Tasks() to service the SPI driver, and the
 *          frame handler runs synchronously without yielding.
 *
 *          Instead, the TLV handler posts a request, and the SdCardThread
 *          picks it up, performs the SYS_FS operation, and sends the
 *          TLV response back via ESP32_SendFrame().
 *
 *          Flow:
 *            1. Esp_HandleFrame() receives file TLV → calls FileMgr_Post()
 *            2. FileMgr_Post() stores the request and returns immediately
 *            3. Esp_HandleFrame() sends a minimal "accepted" reply
 *            4. SdCardThread calls FileMgr_Process() each loop pass
 *            5. FileMgr_Process() does the SYS_FS work, builds TLV,
 *               sends result via ESP32_SendFrame()
 */

#ifndef SD_FILE_MGR_H
#define SD_FILE_MGR_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* TLV tags for file operations */
#define T_FILE_LIST     0x70
#define T_FILE_ENTRY    0x71
#define T_FILE_DELETE   0x72
#define T_FILE_RENAME   0x73
#define T_FILE_MKDIR    0x74
#define T_FILE_INFO     0x75
#define T_FILE_CREATE   0x76
#define T_FILE_STATUS   0x77
#define T_FILE_STORAGE  0x78    /**< GET: SD card total/used/free in KB     */

/* Status codes */
#define FMGR_OK          0x00
#define FMGR_NOT_MOUNTED 0x01
#define FMGR_NOT_FOUND   0x02
#define FMGR_IO_ERROR    0x03
#define FMGR_BAD_PARAM   0x04
#define FMGR_BUSY        0x05

/* File entry types */
#define FENTRY_FILE     0x00
#define FENTRY_DIR      0x01

/**
 * @brief  Post a file operation request (non-blocking).
 *         Called from Esp_HandleFrame() context.
 *
 * @param  op    Operation tag (T_FILE_LIST, T_FILE_DELETE, etc.)
 * @param  data  Path or parameter data (copied internally)
 * @param  len   Length of data
 * @return true if accepted, false if busy
 */
bool FileMgr_Post(uint8_t op, const uint8_t *data, size_t len);

/**
 * @brief  Check if there's a pending request.
 */
bool FileMgr_HasPending(void);

/**
 * @brief  Process the pending request (blocking SYS_FS calls OK here).
 *         Called from SdCardThread which can yield to SYS_Tasks().
 *         Sends the TLV response via ESP32_SendFrame().
 */
void FileMgr_Process(void);

#endif /* SD_FILE_MGR_H */
