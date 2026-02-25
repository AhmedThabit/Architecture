/**
 * @file    gsm_le910.h
 * @brief   Lightweight Telit LE410C4/LE910Cx-class AT driver (SMS, voice, signal, URCs)
 *
 * @details
 *  - Blocking high-level calls with timeouts (init, send SMS, read/delete SMS, dial, hangup)
 *  - Non-blocking URC processing via GSM_Task() and user callbacks (+CMTI, +CEREG, RING, etc.)
 *  - Minimal heap use; buffers allocated by caller (PIC32-friendly)
 *
 *  Transport is provided by the application via function pointers (UART write/read, timing).
 *
 *  References:
 *   ? 3GPP TS 27.005 (SMS AT commands)
 *   ? Telit LE910Cx AT Commands Reference (D, CMGF, CNMI, CMGR/CMGS/CMGD, CSQ, CxREG)
 */

#ifndef GSM_LE910_H
#define GSM_LE910_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================= Public configuration ========================= */

/** Default timeouts (ms) ? tune for your network/SIM */
#ifndef GSM_DEFAULT_AT_TIMEOUT_MS
#define GSM_DEFAULT_AT_TIMEOUT_MS       1500u
#endif
#ifndef GSM_BOOT_NETWORK_TIMEOUT_MS
#define GSM_BOOT_NETWORK_TIMEOUT_MS     60000u  /**< Wait up to 60s for CxREG=1/5 */
#endif
#ifndef GSM_SMS_SEND_TIMEOUT_MS
#define GSM_SMS_SEND_TIMEOUT_MS         30000u  /**< Wait for +CMGS/OK */
#endif
#ifndef GSM_SMS_READ_TIMEOUT_MS
#define GSM_SMS_READ_TIMEOUT_MS         2000u
#endif

/** Max line length (excluding CRLF). Used by the internal line assembler. */
#ifndef GSM_MAX_LINE_LEN
#define GSM_MAX_LINE_LEN                192u
#endif

/** Size of the temporary AT RX buffer (raw bytes from UART). */
#ifndef GSM_RX_TMP_CAPACITY
#define GSM_RX_TMP_CAPACITY             256u
#endif

/* ============================ Public types ============================== */

/** Result/error codes returned by API calls. */
typedef enum {
    GSM_OK = 0,
    GSM_ERR_TIMEOUT,
    GSM_ERR_IO,
    GSM_ERR_PARSE,
    GSM_ERR_BUSY,
    GSM_ERR_PARAM,
    GSM_ERR_NOT_READY,
    GSM_ERR_NO_SIM,
    GSM_ERR_NOT_REGISTERED,
    GSM_ERR_CME,      /**< Module returned +CME ERROR with a code */
    GSM_ERR_CMS,      /**< SMS error (+CMS ERROR) */
} GSM_Result;

/** Network**/


 
#endif 