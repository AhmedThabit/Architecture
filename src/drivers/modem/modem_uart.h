/**
 * @file    modem_uart.h
 * @brief   Tier 1 — Modem UART transport layer.
 *
 * @details Owns the physical UART connection to the cellular modem.
 *          Provides raw byte send/receive and a line-oriented ring buffer.
 *          Has ZERO knowledge of AT commands — just byte transport.
 *
 *          To change the UART peripheral (e.g. UART3 → UART2), edit
 *          only the CFG_MODEM_UART_xxx macros in config/app_config.h.
 */

#ifndef MODEM_UART_H
#define MODEM_UART_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Initialisation ─────────────────────────────────────────────────────── */

/**
 * @brief  Initialise the modem UART.
 *         Registers the ISR callback, enables RX notifications,
 *         and clears the internal line ring buffer.
 */
void Modem_Uart_Init(void);

/* ── Transmit ───────────────────────────────────────────────────────────── */

/**
 * @brief  Send raw bytes to the modem.
 * @param  data  pointer to bytes
 * @param  len   number of bytes
 * @return number of bytes actually queued
 */
size_t Modem_Uart_Send(const uint8_t *data, size_t len);

/**
 * @brief  Send a null-terminated string to the modem (no CRLF appended).
 * @param  str   string to send
 */
void Modem_Uart_SendString(const char *str);

/**
 * @brief  Check whether the UART TX buffer has fully drained.
 * @return true when all bytes have been physically shifted out.
 */
bool Modem_Uart_TxComplete(void);

/* ── Receive (line-oriented ring buffer) ────────────────────────────────── */

/**
 * @brief  Feed raw bytes from the UART ISR into the line assembler.
 *         Called automatically by the ISR callback — not for application use.
 * @param  data  pointer to received bytes
 * @param  len   number of bytes
 */
void Modem_Uart_FeedRx(const uint8_t *data, size_t len);

/**
 * @brief  Check whether a complete line (CR/LF terminated) is available.
 * @return true if at least one line is waiting.
 */
bool Modem_Uart_HasLine(void);

/**
 * @brief  Retrieve the oldest complete line from the ring buffer.
 *         CR/LF characters are stripped.
 * @return pointer to an internal buffer (valid until next call), or NULL.
 */
const char *Modem_Uart_GetLine(void);

/**
 * @brief  Discard all buffered lines and reset the line assembler.
 */
void Modem_Uart_FlushRx(void);

/* ── Raw read (for accumulator-style polling) ──────────────────────────── */

/**
 * @brief  Read raw bytes from the UART RX FIFO (non-blocking).
 * @param  buf   destination buffer
 * @param  max   maximum bytes to read
 * @return number of bytes actually read
 */
size_t Modem_Uart_ReadRaw(uint8_t *buf, size_t max);

#endif /* MODEM_UART_H */
