/**
 * @file    modem_at.c
 * @brief   Tier 2 — Generic AT command engine implementation.
 *
 * @details Uses Modem_Uart (Tier 1) for all byte I/O.
 *          Implements the accumulator + token-search pattern used
 *          by the protothread-based service layer.
 */

#include "modem_at.h"
#include "modem_uart.h"
#include "config/app_config.h"

#include <string.h>

/* ── Tick source (from BSP Timer1 ISR) ──────────────────────────────────── */
extern volatile uint32_t g_ms_ticks;

/* ── Private state ──────────────────────────────────────────────────────── */

static char     s_acc[CFG_MODEM_RX_RAW_BUF];
static uint16_t s_acc_len;
static uint32_t s_deadline_start;
static uint32_t s_deadline_ms;

/* ── Private helpers ────────────────────────────────────────────────────── */

static void acc_pump(void)
{
    uint8_t tmp[128];
    size_t n;
    while ((n = Modem_Uart_ReadRaw(tmp, sizeof(tmp))) > 0) {
        if ((uint16_t)(s_acc_len + n) >= sizeof(s_acc)) {
            s_acc_len = 0;
        }
        memcpy(&s_acc[s_acc_len], tmp, n);
        s_acc_len += (uint16_t)n;
        s_acc[s_acc_len] = '\0';
    }
}

static inline bool deadline_elapsed(void)
{
    return ((uint32_t)(g_ms_ticks - s_deadline_start) >= s_deadline_ms);
}

/* ── Public API ─────────────────────────────────────────────────────────── */

void Modem_AT_Init(void)
{
    s_acc_len = 0;
    s_acc[0] = '\0';
    s_deadline_start = 0;
    s_deadline_ms = 0;
}

void Modem_AT_BeginExchange(uint32_t timeout_ms)
{
    s_acc_len = 0;
    s_acc[0] = '\0';
    s_deadline_start = g_ms_ticks;
    s_deadline_ms = timeout_ms;
}

void Modem_AT_SendCmd(const char *cmd)
{
    Modem_Uart_SendString(cmd);
    Modem_Uart_SendString("\r\n");
}

void Modem_AT_SendRaw(const uint8_t *data, size_t len)
{
    Modem_Uart_Send(data, len);
}

bool Modem_AT_WaitToken(const char *token, bool *found)
{
    acc_pump();

    if (strstr(s_acc, token)) {
        if (found) *found = true;
        return true;
    }
    if (deadline_elapsed()) {
        if (found) *found = false;
        return true;
    }
    return false;
}

bool Modem_AT_WaitAny(const char *const tokens[], int count, int *which)
{
    acc_pump();

    for (int i = 0; i < count; i++) {
        if (strstr(s_acc, tokens[i])) {
            if (which) *which = i;
            return true;
        }
    }
    if (deadline_elapsed()) {
        if (which) *which = -1;
        return true;
    }
    return false;
}

const char *Modem_AT_GetAccumulator(void)
{
    acc_pump();
    return s_acc;
}

uint16_t Modem_AT_GetAccumulatorLen(void)
{
    return s_acc_len;
}

/* ── URC Parsers ────────────────────────────────────────────────────────── */

bool Modem_AT_WaitCPIN(int *result)
{
    acc_pump();

    char *p = strstr(s_acc, "+CPIN:");
    if (p) {
        p += 6;
        while (*p == ' ' || *p == '\t') p++;

        const char *s = p;
        while (*p && *p != '\r' && *p != '\n') p++;
        size_t n = (size_t)(p - s);
        while (n && (s[n - 1] == ' ' || s[n - 1] == '\t')) n--;

        int res = 3;    /* other / error */
        if (n == 5 && memcmp(s, "READY", 5) == 0)   res = 0;
        else if (n == 7 && memcmp(s, "SIM PIN", 7) == 0) res = 1;
        else if (n == 7 && memcmp(s, "SIM PUK", 7) == 0) res = 2;

        if (result) *result = res;

        /* consume the line from the accumulator */
        while (*p && *p != '\n') p++;
        if (*p == '\n') p++;
        size_t cut = (size_t)(p - s_acc);
        memmove(s_acc, s_acc + cut, s_acc_len + 1 - cut);
        s_acc_len -= (s_acc_len >= cut) ? (uint16_t)cut : s_acc_len;

        return true;
    }

    if (deadline_elapsed()) {
        if (result) *result = -1;
        return true;
    }
    return false;
}

bool Modem_AT_WaitCREG(bool *registered)
{
    acc_pump();

    char *p = strstr(s_acc, "+CREG:");
    if (!p) p = strstr(s_acc, "+CEREG:");

    if (p) {
        /* find end of line */
        char *eol = p;
        while (*eol && *eol != '\n') eol++;
        if (*eol == '\n') eol++;

        /* check for ,1 or ,5 (registered home / roaming) */
        bool ok = (strstr(s_acc, ",1") || strstr(s_acc, ",5")) != (void*)0;
        if (registered) *registered = ok;

        /* consume */
        size_t cut = (size_t)(eol - s_acc);
        memmove(s_acc, s_acc + cut, s_acc_len + 1 - cut);
        s_acc_len -= (s_acc_len >= cut) ? (uint16_t)cut : s_acc_len;

        return true;
    }

    if (deadline_elapsed()) {
        if (registered) *registered = false;
        return true;
    }
    return false;
}
