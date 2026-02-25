/**
 * @file    gsm_service.c
 * @brief   Tier 3 — Telit LE910C4 GSM service implementation.
 *
 * @details Contains the actual AT command strings for the LE910C4.
 *          ╔═══════════════════════════════════════════════════════════╗
 *          ║  TO SWITCH MODEM:  Replace the AT strings in this file.  ║
 *          ║  Tiers 1 (modem_uart) and 2 (modem_at) stay unchanged.  ║
 *          ╚═══════════════════════════════════════════════════════════╝
 *
 *          Every public function follows one of two patterns:
 *            1. GSM_Cmd_Xxx()  — sends the AT command and begins the exchange
 *            2. GSM_Poll_Xxx() — polls for the response (for PT_WAIT_UNTIL)
 *
 * @note    Reference: Telit LE910Cx AT Commands Reference Guide, Rev. 18
 */

#include "gsm_service.h"
#include "drivers/modem/modem_at.h"
#include "drivers/modem/modem_uart.h"
#include "config/app_config.h"

#include <stdio.h>
#include <string.h>

/* ══════════════════════════════════════════════════════════════════════════
 *  Modem-specific AT command strings — EDIT THESE when swapping hardware
 * ══════════════════════════════════════════════════════════════════════════ */

#if defined(CFG_MODEM_TELIT_LE910C4)
    /* ── Telit LE910C4-EU (4G Cat-4, Linux firmware) ─────────────────── */
    #define AT_ECHO_OFF         "ATE0"
    #define AT_VERBOSE_ERRORS   "AT+CMEE=2"
    #define AT_QUERY_SIM        "AT+CPIN?"
    #define AT_QUERY_REG        "AT+CREG?"
    #define AT_SMS_TEXT_MODE    "AT+CMGF=1"
    #define AT_SMS_CHARSET      "AT+CSCS=\"GSM\""
    #define AT_SMS_NOTIFY       "AT+CNMI=2,1,0,0,0"
    #define AT_SMS_STORAGE      "AT+CPMS=\"ME\",\"ME\",\"ME\""
    #define AT_SIGNAL_QUERY     "AT+CSQ"
    #define AT_HANGUP           "ATH"
    #define AT_USB_AUDIO_CFG    "AT#USBCFG=11"
    #define AT_FUNC_FULL        "AT+CFUN=1"

/*
 * Future modem support — uncomment the appropriate block:
 *
 * #elif defined(CFG_MODEM_TELIT_FN990A28)
 *     // Telit FN990A28 (5G Sub-6)
 *     #define AT_ECHO_OFF         "ATE0"
 *     #define AT_VERBOSE_ERRORS   "AT+CMEE=2"
 *     #define AT_QUERY_SIM        "AT+CPIN?"
 *     #define AT_QUERY_REG        "AT+C5GREG?"      // 5G NR registration
 *     #define AT_SMS_TEXT_MODE    "AT+CMGF=1"
 *     #define AT_SMS_CHARSET      "AT+CSCS=\"GSM\""
 *     #define AT_SMS_NOTIFY       "AT+CNMI=2,1,0,0,0"
 *     #define AT_SMS_STORAGE      "AT+CPMS=\"ME\",\"ME\",\"ME\""
 *     #define AT_SIGNAL_QUERY     "AT+CESQ"         // Extended signal quality
 *     #define AT_HANGUP           "ATH"
 *     #define AT_FUNC_FULL        "AT+CFUN=1"
 *
 * #elif defined(CFG_MODEM_QUECTEL_EG25)
 *     // Quectel EG25-G (4G Cat-4, widely available)
 *     #define AT_ECHO_OFF         "ATE0"
 *     #define AT_VERBOSE_ERRORS   "AT+CMEE=2"
 *     #define AT_QUERY_SIM        "AT+CPIN?"
 *     #define AT_QUERY_REG        "AT+CEREG?"
 *     #define AT_SMS_TEXT_MODE    "AT+CMGF=1"
 *     #define AT_SMS_CHARSET      "AT+CSCS=\"GSM\""
 *     #define AT_SMS_NOTIFY       "AT+CNMI=2,1,0,0,0"
 *     #define AT_SMS_STORAGE      "AT+CPMS=\"ME\",\"ME\",\"ME\""
 *     #define AT_SIGNAL_QUERY     "AT+CSQ"
 *     #define AT_HANGUP           "ATH"
 *     #define AT_FUNC_FULL        "AT+CFUN=1"
 */

#else
    #error "No modem type defined in app_config.h — define CFG_MODEM_TELIT_LE910C4 or equivalent"
#endif

/* ══════════════════════════════════════════════════════════════════════════
 *  Initialisation
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Service_Init(void)
{
    Modem_Uart_Init();
    Modem_AT_Init();
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Preflight commands
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Cmd_EchoOff(void)
{
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(AT_ECHO_OFF);
}

void GSM_Cmd_VerboseErrors(void)
{
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(AT_VERBOSE_ERRORS);
}

void GSM_Cmd_QuerySIM(void)
{
    Modem_AT_BeginExchange(4000);
    Modem_AT_SendCmd(AT_QUERY_SIM);
}

bool GSM_Poll_SIMStatus(GSM_SimStatus *status)
{
    int raw = -1;
    bool done = Modem_AT_WaitCPIN(&raw);
    if (done && status) {
        *status = (GSM_SimStatus)raw;
    }
    return done;
}

void GSM_Cmd_QueryRegistration(void)
{
    Modem_AT_BeginExchange(2000);
    Modem_AT_SendCmd(AT_QUERY_REG);
}

bool GSM_Poll_Registration(bool *registered)
{
    return Modem_AT_WaitCREG(registered);
}

void GSM_Cmd_ConfigSMS(void)
{
    Modem_AT_BeginExchange(3000);
    Modem_AT_SendCmd(AT_SMS_TEXT_MODE);
}

bool GSM_Poll_ConfigSMS(int *which)
{
    const char *tokens[] = { "OK", "ERROR" };
    return Modem_AT_WaitAny(tokens, 2, which);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  SMS operations
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Cmd_BeginSMS(const char *number)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"", number);
    Modem_AT_BeginExchange(8000);
    Modem_AT_SendCmd(cmd);
}

bool GSM_Poll_SMSPrompt(int *which)
{
    const char *tokens[] = { ">", "CONNECT", "ERROR" };
    return Modem_AT_WaitAny(tokens, 3, which);
}

void GSM_Cmd_SendSMSBody(const char *text)
{
    Modem_Uart_SendString(text);
    /* Wait for TX to drain before sending Ctrl-Z */
    /* (caller should PT_WAIT_UNTIL Modem_Uart_TxComplete() first) */
    uint8_t ctrl_z = 0x1A;
    Modem_AT_BeginExchange(CFG_MODEM_SMS_SEND_TIMEOUT);
    Modem_AT_SendRaw(&ctrl_z, 1);
}

bool GSM_Poll_SMSResult(int *which)
{
    const char *tokens[] = { "OK", "ERROR" };
    return Modem_AT_WaitAny(tokens, 2, which);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Voice call
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Cmd_Dial(const char *number)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ATD%s;", number);
    Modem_AT_BeginExchange(CFG_MODEM_BOOT_TIMEOUT_MS);
    Modem_AT_SendCmd(cmd);
}

void GSM_Cmd_Hangup(void)
{
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(AT_HANGUP);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Signal quality
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Cmd_QuerySignal(void)
{
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(AT_SIGNAL_QUERY);
}

bool GSM_Poll_Signal(int8_t *rssi_dbm)
{
    bool found = false;
    bool done = Modem_AT_WaitToken("+CSQ:", &found);

    if (done && found && rssi_dbm) {
        const char *acc = Modem_AT_GetAccumulator();
        const char *p = strstr(acc, "+CSQ:");
        if (p) {
            p += 5;
            while (*p == ' ') p++;
            int csq = 0;
            while (*p >= '0' && *p <= '9') {
                csq = csq * 10 + (*p - '0');
                p++;
            }
            /* Convert CSQ (0-31) to approximate dBm: -113 + 2*csq */
            if (csq <= 31) {
                *rssi_dbm = (int8_t)(-113 + 2 * csq);
            } else {
                *rssi_dbm = -128;  /* unknown */
            }
        }
    } else if (done && !found && rssi_dbm) {
        *rssi_dbm = -128;
    }

    return done;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  USB Audio (LE910C4 Linux-specific)
 * ══════════════════════════════════════════════════════════════════════════ */

#if defined(CFG_MODEM_TELIT_LE910C4)

void GSM_Cmd_ConfigUSBAudio(void)
{
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(AT_USB_AUDIO_CFG);
}

void GSM_Cmd_PlayAudio(const char *filename)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT#APLAY=\"%s\"", filename);
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(cmd);
}

void GSM_Cmd_SendAudio(const uint8_t *data, size_t len)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT#ASEND=%u", (unsigned)len);
    Modem_AT_BeginExchange(CFG_MODEM_AT_TIMEOUT_MS);
    Modem_AT_SendCmd(cmd);
    /* Caller should wait for CONNECT prompt, then send raw audio data */
    (void)data; /* actual data send is done by caller after prompt */
}

#endif /* CFG_MODEM_TELIT_LE910C4 */

/* ══════════════════════════════════════════════════════════════════════════
 *  Generic AT (ping / keep-alive)
 * ══════════════════════════════════════════════════════════════════════════ */

void GSM_Cmd_Ping(uint32_t timeout_ms)
{
    Modem_AT_BeginExchange(timeout_ms);
    Modem_AT_SendCmd("AT");
}

bool GSM_Poll_OkError(int *which)
{
    const char *tokens[] = { "OK", "ERROR" };
    return Modem_AT_WaitAny(tokens, 2, which);
}
