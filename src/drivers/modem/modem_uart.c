/**
 * @file    modem_uart.c
 * @brief   Tier 1 — Modem UART transport implementation.
 *
 * @details All hardware-specific UART calls are routed through
 *          CFG_MODEM_UART_xxx macros from app_config.h.
 *          To rewire the modem to a different UART, change those macros only.
 */

#include "modem_uart.h"
#include "config/app_config.h"

/* Harmony PLIB headers — needed for the callback type definition */
#include "peripheral/uart/plib_uart_common.h"
#include "peripheral/uart/plib_uart3.h"

#include <string.h>

/* ── Line ring buffer (private) ─────────────────────────────────────────── */

static char s_line_buf[CFG_MODEM_RX_LINE_MAX];
static uint16_t s_line_pos;

static char s_lines[CFG_MODEM_RX_LINES][CFG_MODEM_RX_LINE_MAX];
static uint8_t s_head;
static uint8_t s_tail;

/* ── ISR callback ───────────────────────────────────────────────────────── */

static void modem_uart_rx_cb(UART_EVENT event, uintptr_t context)
{
    (void)context;
    if (event == UART_EVENT_READ_THRESHOLD_REACHED ||
        event == UART_EVENT_READ_BUFFER_FULL)
    {
        uint8_t tmp[64];
        size_t n = CFG_MODEM_UART_READ(tmp, sizeof(tmp));
        if (n > 0) {
            Modem_Uart_FeedRx(tmp, n);
        }
    }
}

/* ── Public API ─────────────────────────────────────────────────────────── */

void Modem_Uart_Init(void)
{
    s_line_pos = 0;
    s_head = 0;
    s_tail = 0;
    memset(s_lines, 0, sizeof(s_lines));

    CFG_MODEM_UART_CB_REG(modem_uart_rx_cb, 0);
    CFG_MODEM_UART_THRESH_SET(1);
    CFG_MODEM_UART_NOTIFY_EN(true, true);
}

size_t Modem_Uart_Send(const uint8_t *data, size_t len)
{
    return CFG_MODEM_UART_WRITE((uint8_t *)data, len);
}

void Modem_Uart_SendString(const char *str)
{
    size_t len = strlen(str);
    CFG_MODEM_UART_WRITE((uint8_t *)str, len);
}

bool Modem_Uart_TxComplete(void)
{
    return CFG_MODEM_UART_TX_COMPLETE();
}

void Modem_Uart_FeedRx(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        char c = (char)data[i];

        if (c == '\r') {
            /* skip carriage return */
        }
        else if (c == '\n') {
            if (s_line_pos > 0) {
                s_line_buf[s_line_pos] = '\0';
                uint8_t next = (s_head + 1) % CFG_MODEM_RX_LINES;
                if (next != s_tail) {
                    strncpy(s_lines[s_head], s_line_buf, CFG_MODEM_RX_LINE_MAX - 1);
                    s_lines[s_head][CFG_MODEM_RX_LINE_MAX - 1] = '\0';
                    s_head = next;
                }
                s_line_pos = 0;
            }
        }
        else {
            if (s_line_pos < CFG_MODEM_RX_LINE_MAX - 1) {
                s_line_buf[s_line_pos++] = c;
            }
        }
    }
}

bool Modem_Uart_HasLine(void)
{
    return (s_head != s_tail);
}

const char *Modem_Uart_GetLine(void)
{
    if (s_head == s_tail) {
        return (void *)0;
    }
    const char *line = s_lines[s_tail];
    s_tail = (s_tail + 1) % CFG_MODEM_RX_LINES;
    return line;
}

void Modem_Uart_FlushRx(void)
{
    s_head = 0;
    s_tail = 0;
    s_line_pos = 0;
}

size_t Modem_Uart_ReadRaw(uint8_t *buf, size_t max)
{
    return CFG_MODEM_UART_READ(buf, max);
}
