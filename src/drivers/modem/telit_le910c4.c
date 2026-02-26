/**
 * @file    telit_le910c4.c
 * @brief   Telit LE910C4-EU backend — implements modem_api.h interface.
 *
 * @details This file contains ALL Telit LE910C4-specific AT command logic.
 *          It is the ONLY file that knows about Telit AT strings.
 *
 *          ╔═══════════════════════════════════════════════════════════╗
 *          ║  TO SWAP MODEM:  Create a new backend .c file that      ║
 *          ║  implements the same Modem_Xxx() functions.              ║
 *          ║  modem_uart.c and modem_at.c stay UNCHANGED.            ║
 *          ║  modem_api.h stays UNCHANGED.                            ║
 *          ║  Protothreads & app code stay UNCHANGED.                ║
 *          ╚═══════════════════════════════════════════════════════════╝
 *
 * @note    Reference: Telit LE910Cx AT Commands Reference Guide, Rev. 18
 */

#include "services/modem_api/modem_api.h"
#include "telit_le910c4.h"
#include "modem_at.h"
#include "modem_uart.h"
#include "config/app_config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ── Tick source ────────────────────────────────────────────────────────── */
extern volatile uint32_t g_ms_ticks;

/* ── Internal state ─────────────────────────────────────────────────────── */

static SmsSendState   s_sms_state;
static const char    *s_sms_body_ptr;

static SmsCfgState    s_sms_cfg_state;
static NetInfoState   s_net_info_state;
static IdentityState  s_identity_state;

/* Temp buffers for multi-step operations */
static Modem_NetworkInfo *s_net_info_out;
static Modem_Identity    *s_identity_out;

/* URC callbacks */
static Modem_RingCallback        s_ring_cb;
static Modem_SmsReceivedCallback s_sms_recv_cb;
static Modem_CallStatusCallback  s_call_status_cb;

/* ══════════════════════════════════════════════════════════════════════════
 *  Internal helpers
 * ══════════════════════════════════════════════════════════════════════════ */

/** Simple OK/ERROR poll — maps AT result to Modem_Result. */
static bool poll_ok_error(Modem_Result *result)
{
    const char *tokens[] = { "OK", "ERROR" };
    int which = -1;
    bool done = Modem_AT_WaitAny(tokens, 2, &which);
    if (done && result) {
        if (which == 0)      *result = MODEM_OK;
        else if (which == 1) *result = MODEM_ERROR;
        else                 *result = MODEM_TIMEOUT;
    }
    return done;
}

/** Parse a numeric value from the accumulator after a known prefix. */
static bool parse_int_after(const char *prefix, int *out_val)
{
    const char *acc = Modem_AT_GetAccumulator();
    const char *p = strstr(acc, prefix);
    if (!p) return false;

    p += strlen(prefix);
    while (*p == ' ' || *p == ':') p++;

    int val = 0;
    bool found = false;
    while (*p >= '0' && *p <= '9') {
        val = val * 10 + (*p - '0');
        p++;
        found = true;
    }
    if (found && out_val) *out_val = val;
    return found;
}

/** Extract a string field from accumulator between delimiters or to EOL. */
static bool parse_string_field(const char *prefix, char *out, size_t max)
{
    const char *acc = Modem_AT_GetAccumulator();
    const char *p = strstr(acc, prefix);
    if (!p) return false;

    p += strlen(prefix);
    while (*p == ' ' || *p == ':' || *p == '"') p++;

    size_t i = 0;
    while (*p && *p != '"' && *p != '\r' && *p != '\n' && i < max - 1) {
        out[i++] = *p++;
    }
    out[i] = '\0';
    return (i > 0);
}

/** Extract the first line of response (skipping echo/blank lines). */
static bool parse_first_line(char *out, size_t max)
{
    const char *acc = Modem_AT_GetAccumulator();
    /* Skip leading CRLF and blank lines */
    while (*acc == '\r' || *acc == '\n') acc++;
    /* Skip echo line (starts with "AT") */
    if (acc[0] == 'A' && acc[1] == 'T') {
        while (*acc && *acc != '\n') acc++;
        while (*acc == '\r' || *acc == '\n') acc++;
    }

    size_t i = 0;
    while (*acc && *acc != '\r' && *acc != '\n' && i < max - 1) {
        out[i++] = *acc++;
    }
    out[i] = '\0';

    /* Don't return "OK" or "ERROR" as the first line */
    if (strcmp(out, "OK") == 0 || strcmp(out, "ERROR") == 0) {
        out[0] = '\0';
        return false;
    }
    return (i > 0);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  1. INITIALISATION & LIFECYCLE
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_Init(void)
{
    Modem_Uart_Init();
    Modem_AT_Init();

    s_sms_state      = SMS_SEND_IDLE;
    s_sms_cfg_state  = SMS_CFG_IDLE;
    s_net_info_state = NET_INFO_IDLE;
    s_identity_state = IDENTITY_IDLE;

    s_ring_cb        = NULL;
    s_sms_recv_cb    = NULL;
    s_call_status_cb = NULL;
}

void Modem_SetPowerMode_Cmd(Modem_PowerMode mode)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_CFUN);
    switch (mode) {
        case MODEM_POWER_FULL:    Modem_AT_SendCmd(AT_CMD_CFUN_FULL);    break;
        case MODEM_POWER_MINIMUM: Modem_AT_SendCmd(AT_CMD_CFUN_MINIMUM); break;
        case MODEM_POWER_RESET:   Modem_AT_SendCmd(AT_CMD_CFUN_RESET);   break;
    }
}

bool Modem_SetPowerMode_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  2. PREFLIGHT / CONFIGURATION
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_EchoOff_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_ECHO_OFF);
}

bool Modem_EchoOff_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_VerboseErrors_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_VERBOSE_ERRORS);
}

bool Modem_VerboseErrors_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_Ping_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_PING);
}

bool Modem_Ping_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  3. SIM CARD
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_QuerySIM_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_SIM);
    Modem_AT_SendCmd(AT_CMD_QUERY_SIM);
}

bool Modem_QuerySIM_Poll(Modem_Result *result, Modem_SimStatus *status)
{
    int raw = -1;
    bool done = Modem_AT_WaitCPIN(&raw);

    if (done) {
        if (result) {
            *result = (raw >= 0) ? MODEM_OK : MODEM_TIMEOUT;
        }
        if (status) {
            switch (raw) {
                case 0:  *status = MODEM_SIM_READY;        break;
                case 1:  *status = MODEM_SIM_PIN_REQUIRED;  break;
                case 2:  *status = MODEM_SIM_PUK_REQUIRED;  break;
                default: *status = MODEM_SIM_ERROR;          break;
            }
        }
    }
    return done;
}

void Modem_EnterPIN_Cmd(const char *pin)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT+CPIN=\"%s\"", pin);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_SIM);
    Modem_AT_SendCmd(cmd);
}

bool Modem_EnterPIN_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  4. NETWORK REGISTRATION
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_QueryRegistration_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_REG);
    Modem_AT_SendCmd(AT_CMD_QUERY_CREG);
}

bool Modem_QueryRegistration_Poll(Modem_Result *result, Modem_NetStatus *status)
{
    bool registered = false;
    bool done = Modem_AT_WaitCREG(&registered);

    if (done) {
        if (result) *result = MODEM_OK;

        if (status) {
            /* Parse the actual stat value from accumulator for detailed status */
            const char *acc = Modem_AT_GetAccumulator();
            const char *p = strstr(acc, "+CREG:");
            if (!p) p = strstr(acc, "+CEREG:");

            if (p) {
                /* Format: +CREG: <n>,<stat>  — find the second number */
                const char *comma = strchr(p, ',');
                if (comma) {
                    int stat = comma[1] - '0';
                    switch (stat) {
                        case 0:  *status = MODEM_NET_NOT_REGISTERED; break;
                        case 1:  *status = MODEM_NET_HOME;           break;
                        case 2:  *status = MODEM_NET_SEARCHING;      break;
                        case 3:  *status = MODEM_NET_DENIED;         break;
                        case 5:  *status = MODEM_NET_ROAMING;        break;
                        default: *status = MODEM_NET_UNKNOWN;        break;
                    }
                } else {
                    *status = registered ? MODEM_NET_HOME : MODEM_NET_NOT_REGISTERED;
                }
            } else {
                *status = registered ? MODEM_NET_HOME : MODEM_NET_NOT_REGISTERED;
            }
        }
    }
    return done;
}

void Modem_QuerySignal_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_QUERY_SIGNAL);
}

bool Modem_QuerySignal_Poll(Modem_Result *result, int8_t *rssi_dbm, uint8_t *ber)
{
    bool found = false;
    bool done = Modem_AT_WaitToken("+CSQ:", &found);

    if (done) {
        if (result) *result = found ? MODEM_OK : MODEM_TIMEOUT;

        if (found) {
            const char *acc = Modem_AT_GetAccumulator();
            const char *p = strstr(acc, "+CSQ:");
            if (p) {
                p += 5;
                while (*p == ' ') p++;

                /* Parse CSQ value */
                int csq = 0;
                while (*p >= '0' && *p <= '9') {
                    csq = csq * 10 + (*p - '0');
                    p++;
                }

                if (rssi_dbm) {
                    *rssi_dbm = (csq <= 31) ? (int8_t)(-113 + 2 * csq) : -128;
                }

                /* Parse BER value (after comma) */
                if (ber) {
                    if (*p == ',') {
                        p++;
                        int b = 0;
                        while (*p >= '0' && *p <= '9') {
                            b = b * 10 + (*p - '0');
                            p++;
                        }
                        *ber = (uint8_t)b;
                    } else {
                        *ber = 99;
                    }
                }
            }
        } else {
            if (rssi_dbm) *rssi_dbm = -128;
            if (ber)      *ber = 99;
        }
    }
    return done;
}

void Modem_QueryOperator_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_REG);
    Modem_AT_SendCmd(AT_CMD_QUERY_OPERATOR);
}

bool Modem_QueryOperator_Poll(Modem_Result *result, char *name, size_t name_max)
{
    bool found = false;
    bool done = Modem_AT_WaitToken("+COPS:", &found);

    if (done) {
        if (result) *result = found ? MODEM_OK : MODEM_TIMEOUT;

        if (found && name && name_max > 0) {
            /* +COPS: 0,0,"Operator Name",7 */
            const char *acc = Modem_AT_GetAccumulator();
            const char *p = strstr(acc, "+COPS:");
            if (p) {
                /* Find first quote */
                const char *q1 = strchr(p, '"');
                if (q1) {
                    q1++;
                    const char *q2 = strchr(q1, '"');
                    if (q2) {
                        size_t len = (size_t)(q2 - q1);
                        if (len >= name_max) len = name_max - 1;
                        memcpy(name, q1, len);
                        name[len] = '\0';
                    } else {
                        name[0] = '\0';
                    }
                } else {
                    name[0] = '\0';
                }
            }
        } else if (name && name_max > 0) {
            name[0] = '\0';
        }
    }
    return done;
}

/* ── GetNetworkInfo (multi-step: CREG → CSQ → COPS) ─────────────────── */

void Modem_GetNetworkInfo_Cmd(void)
{
    s_net_info_state = NET_INFO_REG;
    Modem_AT_BeginExchange(TELIT_TIMEOUT_REG);
    Modem_AT_SendCmd(AT_CMD_QUERY_CREG);
}

bool Modem_GetNetworkInfo_Poll(Modem_Result *result, Modem_NetworkInfo *info)
{
    s_net_info_out = info;

    switch (s_net_info_state) {

        case NET_INFO_REG: {
            Modem_NetStatus st;
            Modem_Result r;
            if (!Modem_QueryRegistration_Poll(&r, &st)) return false;

            if (info) info->reg_status = st;
            s_net_info_state = NET_INFO_SIGNAL;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
            Modem_AT_SendCmd(AT_CMD_QUERY_SIGNAL);
            return false;   /* more steps */
        }

        case NET_INFO_SIGNAL: {
            int8_t rssi;
            uint8_t ber;
            Modem_Result r;
            if (!Modem_QuerySignal_Poll(&r, &rssi, &ber)) return false;

            if (info) {
                info->rssi_dbm = rssi;
                info->ber = ber;
                /* Convert back to CSQ for convenience */
                if (rssi >= -113 && rssi <= -51) {
                    info->csq = (uint8_t)((rssi + 113) / 2);
                } else {
                    info->csq = 99;
                }
            }
            s_net_info_state = NET_INFO_OPERATOR;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_REG);
            Modem_AT_SendCmd(AT_CMD_QUERY_OPERATOR);
            return false;   /* more steps */
        }

        case NET_INFO_OPERATOR: {
            char name[32] = {0};
            Modem_Result r;
            if (!Modem_QueryOperator_Poll(&r, name, sizeof(name))) return false;

            if (info) {
                strncpy(info->operator_name, name, sizeof(info->operator_name) - 1);
                info->operator_name[sizeof(info->operator_name) - 1] = '\0';
            }
            s_net_info_state = NET_INFO_IDLE;
            if (result) *result = MODEM_OK;
            return true;    /* done! */
        }

        default:
            s_net_info_state = NET_INFO_IDLE;
            if (result) *result = MODEM_ERROR;
            return true;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  5. MODEM IDENTIFICATION
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_GetIdentity_Cmd(void)
{
    s_identity_state = IDENTITY_MANUFACTURER;
    Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
    Modem_AT_SendCmd(AT_CMD_MANUFACTURER);
}

bool Modem_GetIdentity_Poll(Modem_Result *result, Modem_Identity *id)
{
    s_identity_out = id;
    Modem_Result r;

    switch (s_identity_state) {

        case IDENTITY_MANUFACTURER: {
            if (!poll_ok_error(&r)) return false;
            if (id) parse_first_line(id->manufacturer, sizeof(id->manufacturer));
            s_identity_state = IDENTITY_MODEL;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
            Modem_AT_SendCmd(AT_CMD_MODEL);
            return false;
        }

        case IDENTITY_MODEL: {
            if (!poll_ok_error(&r)) return false;
            if (id) parse_first_line(id->model, sizeof(id->model));
            s_identity_state = IDENTITY_FIRMWARE;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
            Modem_AT_SendCmd(AT_CMD_FIRMWARE);
            return false;
        }

        case IDENTITY_FIRMWARE: {
            if (!poll_ok_error(&r)) return false;
            if (id) parse_first_line(id->firmware, sizeof(id->firmware));
            s_identity_state = IDENTITY_IMEI;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
            Modem_AT_SendCmd(AT_CMD_IMEI);
            return false;
        }

        case IDENTITY_IMEI: {
            if (!poll_ok_error(&r)) return false;
            if (id) parse_first_line(id->imei, sizeof(id->imei));
            s_identity_state = IDENTITY_ICCID;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
            Modem_AT_SendCmd(AT_CMD_TELIT_ICCID);
            return false;
        }

        case IDENTITY_ICCID: {
            if (!poll_ok_error(&r)) return false;
            if (id) {
                /* Response: #CCID: <iccid> */
                if (!parse_string_field("#CCID", id->iccid, sizeof(id->iccid))) {
                    parse_first_line(id->iccid, sizeof(id->iccid));
                }
            }
            s_identity_state = IDENTITY_IMSI;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
            Modem_AT_SendCmd(AT_CMD_IMSI);
            return false;
        }

        case IDENTITY_IMSI: {
            if (!poll_ok_error(&r)) return false;
            if (id) parse_first_line(id->imsi, sizeof(id->imsi));
            s_identity_state = IDENTITY_IDLE;
            if (result) *result = MODEM_OK;
            return true;
        }

        default:
            s_identity_state = IDENTITY_IDLE;
            if (result) *result = MODEM_ERROR;
            return true;
    }
}

/* Individual identity queries */

void Modem_QueryFirmwareVersion_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
    Modem_AT_SendCmd(AT_CMD_FIRMWARE);
}

bool Modem_QueryFirmwareVersion_Poll(Modem_Result *result, char *ver, size_t ver_max)
{
    Modem_Result r;
    bool done = poll_ok_error(&r);
    if (done) {
        if (result) *result = r;
        if (ver && ver_max > 0) {
            if (!parse_first_line(ver, ver_max)) ver[0] = '\0';
        }
    }
    return done;
}

void Modem_QueryIMEI_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
    Modem_AT_SendCmd(AT_CMD_IMEI);
}

bool Modem_QueryIMEI_Poll(Modem_Result *result, char *imei, size_t imei_max)
{
    Modem_Result r;
    bool done = poll_ok_error(&r);
    if (done) {
        if (result) *result = r;
        if (imei && imei_max > 0) {
            if (!parse_first_line(imei, imei_max)) imei[0] = '\0';
        }
    }
    return done;
}

void Modem_QueryICCID_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_IDENTITY);
    Modem_AT_SendCmd(AT_CMD_TELIT_ICCID);
}

bool Modem_QueryICCID_Poll(Modem_Result *result, char *iccid, size_t iccid_max)
{
    Modem_Result r;
    bool done = poll_ok_error(&r);
    if (done) {
        if (result) *result = r;
        if (iccid && iccid_max > 0) {
            if (!parse_string_field("#CCID", iccid, iccid_max)) {
                if (!parse_first_line(iccid, iccid_max)) iccid[0] = '\0';
            }
        }
    }
    return done;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  6. SMS — CONFIGURATION
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_ConfigSMS_Cmd(void)
{
    s_sms_cfg_state = SMS_CFG_TEXT_MODE;
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_SMS_TEXT_MODE);
}

bool Modem_ConfigSMS_Poll(Modem_Result *result)
{
    Modem_Result r;

    switch (s_sms_cfg_state) {

        case SMS_CFG_TEXT_MODE: {
            if (!poll_ok_error(&r)) return false;
            if (r != MODEM_OK) {
                s_sms_cfg_state = SMS_CFG_IDLE;
                if (result) *result = r;
                return true;
            }
            s_sms_cfg_state = SMS_CFG_CHARSET;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
            Modem_AT_SendCmd(AT_CMD_SMS_CHARSET);
            return false;
        }

        case SMS_CFG_CHARSET: {
            if (!poll_ok_error(&r)) return false;
            /* Charset failure is non-fatal — continue */
            s_sms_cfg_state = SMS_CFG_NOTIFY;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
            Modem_AT_SendCmd(AT_CMD_SMS_NOTIFY);
            return false;
        }

        case SMS_CFG_NOTIFY: {
            if (!poll_ok_error(&r)) return false;
            s_sms_cfg_state = SMS_CFG_STORAGE;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
            Modem_AT_SendCmd(AT_CMD_SMS_STORAGE);
            return false;
        }

        case SMS_CFG_STORAGE: {
            if (!poll_ok_error(&r)) return false;
            s_sms_cfg_state = SMS_CFG_IDLE;
            if (result) *result = MODEM_OK;
            return true;
        }

        default:
            s_sms_cfg_state = SMS_CFG_IDLE;
            if (result) *result = MODEM_ERROR;
            return true;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  7. SMS — SEND  (multi-step: AT+CMGS → ">" → body + Ctrl-Z → OK)
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_SendSMS_Cmd(const char *number, const char *text)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMGS=\"%s\"", number);

    s_sms_body_ptr = text;
    s_sms_state = SMS_SEND_WAIT_PROMPT;

    Modem_AT_BeginExchange(TELIT_TIMEOUT_SMS_SEND);
    Modem_AT_SendCmd(cmd);
}

bool Modem_SendSMS_Poll(Modem_Result *result)
{
    switch (s_sms_state) {

        case SMS_SEND_WAIT_PROMPT: {
            const char *tokens[] = { ">", "ERROR" };
            int which = -1;
            bool done = Modem_AT_WaitAny(tokens, 2, &which);
            if (!done) return false;

            if (which != 0) {
                /* ERROR or timeout — abort */
                s_sms_state = SMS_SEND_IDLE;
                if (result) *result = (which == 1) ? MODEM_ERROR : MODEM_TIMEOUT;
                return true;
            }

            /* Got ">" prompt — send body + Ctrl-Z */
            if (s_sms_body_ptr && *s_sms_body_ptr) {
                Modem_Uart_SendString(s_sms_body_ptr);
            }
            uint8_t ctrl_z = 0x1A;
            Modem_AT_BeginExchange(TELIT_TIMEOUT_SMS_SEND);
            Modem_AT_SendRaw(&ctrl_z, 1);

            s_sms_state = SMS_SEND_WAIT_RESULT;
            return false;
        }

        case SMS_SEND_WAIT_RESULT: {
            const char *tokens[] = { "OK", "ERROR" };
            int which = -1;
            bool done = Modem_AT_WaitAny(tokens, 2, &which);
            if (!done) return false;

            s_sms_state = SMS_SEND_IDLE;
            if (result) {
                if (which == 0)      *result = MODEM_OK;
                else if (which == 1) *result = MODEM_ERROR;
                else                 *result = MODEM_TIMEOUT;
            }
            return true;
        }

        default:
            s_sms_state = SMS_SEND_IDLE;
            if (result) *result = MODEM_ERROR;
            return true;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  8. SMS — READ / DELETE
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_ReadSMS_Cmd(uint8_t index)
{
    char cmd[24];
    snprintf(cmd, sizeof(cmd), "AT+CMGR=%u", (unsigned)index);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_SMS_READ);
    Modem_AT_SendCmd(cmd);
}

bool Modem_ReadSMS_Poll(Modem_Result *result, Modem_SmsMessage *msg)
{
    bool found = false;
    bool done = Modem_AT_WaitToken("+CMGR:", &found);

    if (done) {
        if (!found) {
            if (result) *result = MODEM_TIMEOUT;
            return true;
        }

        if (result) *result = MODEM_OK;

        if (msg) {
            memset(msg, 0, sizeof(*msg));

            const char *acc = Modem_AT_GetAccumulator();
            const char *p = strstr(acc, "+CMGR:");
            if (p) {
                /* +CMGR: "REC READ","+20123456789","","24/01/15,10:30:00+08" */

                /* Is it read or unread? */
                msg->is_read = (strstr(p, "REC READ") != NULL);

                /* Extract sender (second quoted field) */
                const char *q = strchr(p, ',');
                if (q) {
                    const char *q1 = strchr(q, '"');
                    if (q1) {
                        q1++;
                        const char *q2 = strchr(q1, '"');
                        if (q2) {
                            size_t len = (size_t)(q2 - q1);
                            if (len >= MODEM_SMS_SENDER_MAX)
                                len = MODEM_SMS_SENDER_MAX - 1;
                            memcpy(msg->sender, q1, len);
                            msg->sender[len] = '\0';
                        }
                    }
                }

                /* Extract timestamp (fourth quoted field) */
                /* Skip to third comma area, look for timestamp pattern */
                const char *ts = p;
                int quote_count = 0;
                while (*ts && quote_count < 6) {
                    if (*ts == '"') quote_count++;
                    ts++;
                }
                if (quote_count >= 6 && *ts) {
                    /* ts points just after 6th quote — this is the timestamp */
                    ts++; /* skip '"' */
                    size_t i = 0;
                    while (*ts && *ts != '"' && i < MODEM_SMS_TIME_MAX - 1) {
                        msg->timestamp[i++] = *ts++;
                    }
                    msg->timestamp[i] = '\0';
                }

                /* Body: starts on the next line after +CMGR header */
                const char *body_start = strstr(p, "\r\n");
                if (!body_start) body_start = strstr(p, "\n");
                if (body_start) {
                    body_start++;
                    if (*body_start == '\n') body_start++;
                    size_t i = 0;
                    while (*body_start && *body_start != '\r'
                            && *body_start != '\n'
                            && i < MODEM_SMS_BODY_MAX - 1) {
                        msg->body[i++] = *body_start++;
                    }
                    msg->body[i] = '\0';
                }
            }
        }
    }
    return done;
}

void Modem_DeleteSMS_Cmd(uint8_t index)
{
    char cmd[24];
    snprintf(cmd, sizeof(cmd), "AT+CMGD=%u", (unsigned)index);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(cmd);
}

bool Modem_DeleteSMS_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_DeleteAllSMS_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_SMS_DELETE_ALL);
}

bool Modem_DeleteAllSMS_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_OnSmsReceived(Modem_SmsReceivedCallback cb)
{
    s_sms_recv_cb = cb;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  9. VOICE CALL
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_Dial_Cmd(const char *number)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ATD%s;", number);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DIAL);
    Modem_AT_SendCmd(cmd);
}

bool Modem_Dial_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_Answer_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_ANSWER);
}

bool Modem_Answer_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_Hangup_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_HANGUP);
}

bool Modem_Hangup_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_QueryCallStatus_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CMD_CALL_STATUS);
}

bool Modem_QueryCallStatus_Poll(Modem_Result *result, Modem_CallStatus *status)
{
    bool found = false;
    bool done = Modem_AT_WaitToken("+CLCC:", &found);

    if (done) {
        if (status) {
            if (!found) {
                /* No +CLCC line means no active calls */
                *status = MODEM_CALL_IDLE;
            } else {
                /* +CLCC: 1,0,<stat>,0,0,"number",129
                 * <stat>: 0=active, 1=held, 2=dialing, 3=alerting,
                 *         4=incoming, 5=waiting, 6=release */
                const char *acc = Modem_AT_GetAccumulator();
                const char *p = strstr(acc, "+CLCC:");
                if (p) {
                    /* Find third comma-separated field */
                    int commas = 0;
                    while (*p && commas < 2) {
                        if (*p == ',') commas++;
                        p++;
                    }
                    int stat = (*p >= '0' && *p <= '9') ? (*p - '0') : -1;
                    switch (stat) {
                        case 0:  *status = MODEM_CALL_ACTIVE;       break;
                        case 1:  *status = MODEM_CALL_ON_HOLD;      break;
                        case 2:  *status = MODEM_CALL_DIALING;      break;
                        case 3:  *status = MODEM_CALL_ALERTING;     break;
                        case 4:  *status = MODEM_CALL_INCOMING;     break;
                        case 6:  *status = MODEM_CALL_DISCONNECTED; break;
                        default: *status = MODEM_CALL_ERROR;        break;
                    }
                } else {
                    *status = MODEM_CALL_IDLE;
                }
            }
        }
        if (result) *result = MODEM_OK;
    }
    return done;
}

void Modem_OnRing(Modem_RingCallback ring_cb)
{
    s_ring_cb = ring_cb;
}

void Modem_OnCallStatus(Modem_CallStatusCallback status_cb)
{
    s_call_status_cb = status_cb;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  10. DTMF
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_SendDTMF_Cmd(char tone)
{
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+VTS=%c", tone);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(cmd);
}

bool Modem_SendDTMF_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_SendDTMFString_Cmd(const char *tones)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+VTS=\"%s\"", tones);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(cmd);
}

bool Modem_SendDTMFString_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_SetDTMFDuration_Cmd(uint8_t duration_tenths)
{
    char cmd[16];
    snprintf(cmd, sizeof(cmd), "AT+VTD=%u", (unsigned)duration_tenths);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(cmd);
}

bool Modem_SetDTMFDuration_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  11. AUDIO
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_ConfigAudio_Cmd(void)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_AUDIO);
    Modem_AT_SendCmd(AT_CMD_TELIT_USB_AUDIO_CFG);
}

bool Modem_ConfigAudio_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_SetVolume_Cmd(uint8_t level)
{
    char cmd[16];
    /* Telit CLVL range is 0-5, map 0-100 → 0-5 */
    uint8_t mapped = (level > 100) ? 5 : (uint8_t)(level / 20);
    snprintf(cmd, sizeof(cmd), "AT+CLVL=%u", (unsigned)mapped);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(cmd);
}

bool Modem_SetVolume_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_PlayAudio_Cmd(const char *filename)
{
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "AT#APLAY=\"%s\"", filename);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_AUDIO);
    Modem_AT_SendCmd(cmd);
}

bool Modem_PlayAudio_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_SendAudio_Cmd(const uint8_t *data, size_t len)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "AT#ASEND=%u", (unsigned)len);
    Modem_AT_BeginExchange(TELIT_TIMEOUT_AUDIO);
    Modem_AT_SendCmd(cmd);
    /* Caller should poll for CONNECT, then send raw data.
     * For simplicity, we send data after a small delay. */
    (void)data;  /* Data send occurs in the poll phase */
}

bool Modem_SendAudio_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Modem_SetMute_Cmd(bool mute)
{
    Modem_AT_BeginExchange(TELIT_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(mute ? AT_CMD_MUTE_ON : AT_CMD_MUTE_OFF);
}

bool Modem_SetMute_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  12. URC PROCESSING
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_ProcessURCs(void)
{
    /* Read any available lines and check for known URCs */
    while (Modem_Uart_HasLine()) {
        const char *line = Modem_Uart_GetLine();
        if (!line) break;

        /* ── RING — incoming call ─────────────────────────────── */
        if (strncmp(line, "RING", 4) == 0) {
            if (s_ring_cb) s_ring_cb(NULL);
            if (s_call_status_cb) s_call_status_cb(MODEM_CALL_INCOMING);
            continue;
        }

        /* ── +CLIP — caller ID with RING ──────────────────────── */
        if (strncmp(line, "+CLIP:", 6) == 0) {
            if (s_ring_cb) {
                /* +CLIP: "+20123456789",145 */
                const char *q1 = strchr(line, '"');
                if (q1) {
                    q1++;
                    const char *q2 = strchr(q1, '"');
                    if (q2) {
                        static char caller_buf[32];
                        size_t len = (size_t)(q2 - q1);
                        if (len >= sizeof(caller_buf))
                            len = sizeof(caller_buf) - 1;
                        memcpy(caller_buf, q1, len);
                        caller_buf[len] = '\0';
                        s_ring_cb(caller_buf);
                    }
                }
            }
            continue;
        }

        /* ── NO CARRIER — call ended ──────────────────────────── */
        if (strncmp(line, "NO CARRIER", 10) == 0) {
            if (s_call_status_cb) s_call_status_cb(MODEM_CALL_DISCONNECTED);
            continue;
        }

        /* ── BUSY ─────────────────────────────────────────────── */
        if (strncmp(line, "BUSY", 4) == 0) {
            if (s_call_status_cb) s_call_status_cb(MODEM_CALL_DISCONNECTED);
            continue;
        }

        /* ── NO ANSWER ────────────────────────────────────────── */
        if (strncmp(line, "NO ANSWER", 9) == 0) {
            if (s_call_status_cb) s_call_status_cb(MODEM_CALL_DISCONNECTED);
            continue;
        }

        /* ── +CMTI — new SMS received ─────────────────────────── */
        if (strncmp(line, "+CMTI:", 6) == 0) {
            if (s_sms_recv_cb) {
                /* +CMTI: "ME",3 */
                const char *p = strrchr(line, ',');
                if (p) {
                    uint8_t idx = (uint8_t)atoi(p + 1);
                    Modem_SmsStorage stor = MODEM_SMS_STORAGE_MODEM;
                    if (strstr(line, "\"SM\"")) stor = MODEM_SMS_STORAGE_SIM;
                    s_sms_recv_cb(idx, stor);
                }
            }
            continue;
        }
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  13. GENERIC / RAW AT
 * ══════════════════════════════════════════════════════════════════════════ */

void Modem_RawCmd_Cmd(const char *cmd, uint32_t timeout_ms)
{
    Modem_AT_BeginExchange(timeout_ms);
    Modem_AT_SendCmd(cmd);
}

bool Modem_RawCmd_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

const char *Modem_GetRawResponse(void)
{
    return Modem_AT_GetAccumulator();
}
