/**
 * @file    modem_at.h
 * @brief   Tier 2 — Generic AT command engine.
 *
 * @details Sends AT commands and collects responses with timeouts.
 *          Knows about OK / ERROR / +CME ERROR / +CMS ERROR patterns
 *          but has NO knowledge of specific modem brands or command sets.
 *
 *          Works with any Hayes-compatible modem (Telit, Quectel, Sierra, u-blox).
 *          Uses Modem_Uart (Tier 1) for byte transport — never touches UART regs.
 */

#ifndef MODEM_AT_H
#define MODEM_AT_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Result codes ───────────────────────────────────────────────────────── */

typedef enum {
    AT_OK = 0,              /**< Modem responded "OK"                      */
    AT_ERROR,               /**< Modem responded "ERROR"                   */
    AT_CME_ERROR,           /**< Modem responded "+CME ERROR: ..."         */
    AT_CMS_ERROR,           /**< Modem responded "+CMS ERROR: ..."         */
    AT_TIMEOUT,             /**< No final response within deadline         */
    AT_BUSY,                /**< Engine is already executing a command      */
} AT_Result;

/* ── Initialisation ─────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the AT engine.  Must be called after Modem_Uart_Init().
 */
void Modem_AT_Init(void);

/* ── Command execution (accumulator + polling style) ────────────────────── */

/**
 * @brief  Begin a new AT exchange: flush buffers and record the deadline.
 * @param  timeout_ms  maximum time to wait for a final response
 */
void Modem_AT_BeginExchange(uint32_t timeout_ms);

/**
 * @brief  Send a raw AT command string (CRLF is appended automatically).
 * @param  cmd   command text, e.g. "AT+CMGF=1"
 */
void Modem_AT_SendCmd(const char *cmd);

/**
 * @brief  Send raw bytes without appending CRLF (e.g. SMS body, Ctrl-Z).
 * @param  data   bytes to send
 * @param  len    number of bytes
 */
void Modem_AT_SendRaw(const uint8_t *data, size_t len);

/**
 * @brief  Poll the accumulator for a specific token.
 *         Call repeatedly from a protothread via PT_WAIT_UNTIL.
 * @param  token   string to search for (e.g. "OK", ">", "+CMGS")
 * @param  found   set to true if token was found before timeout
 * @return true when the wait is complete (either found or timed out)
 */
bool Modem_AT_WaitToken(const char *token, bool *found);

/**
 * @brief  Poll for any of several tokens simultaneously.
 * @param  tokens  array of token strings
 * @param  count   number of tokens
 * @param  which   set to the index of the matched token, or -1 on timeout
 * @return true when the wait is complete
 */
bool Modem_AT_WaitAny(const char *const tokens[], int count, int *which);

/**
 * @brief  Get a read-only pointer to the raw accumulator contents.
 *         Useful for parsing URC data after a successful wait.
 * @return pointer to internal null-terminated buffer
 */
const char *Modem_AT_GetAccumulator(void);

/**
 * @brief  Get the current length of accumulated data.
 */
uint16_t Modem_AT_GetAccumulatorLen(void);

/* ── URC detection helpers ──────────────────────────────────────────────── */

/**
 * @brief  Check the accumulator for a +CPIN: response.
 * @param  result  0=READY, 1=SIM PIN, 2=SIM PUK, 3=other, -1=timeout
 * @return true when a +CPIN line was found or timeout elapsed
 */
bool Modem_AT_WaitCPIN(int *result);

/**
 * @brief  Check the accumulator for +CREG/+CEREG registration.
 * @param  registered  set to true if stat == 1 or 5
 * @return true when response found or timeout elapsed
 */
bool Modem_AT_WaitCREG(bool *registered);

#endif /* MODEM_AT_H */
