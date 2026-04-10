/**
 * @file    telit_le910c4.h
 * @brief   Telit LE910C4-EU AT command strings and internal definitions.
 *
 * @details This is a PRIVATE header — only telit_le910c4.c includes it.
 *          Application code never sees this file.
 *
 *          To add a new modem backend:
 *            1. Create drivers/modem/newmodem_backend.h + .c
 *            2. Define the AT strings for the new modem
 *            3. Implement all Modem_Xxx functions from modem_api.h
 *
 * @note    Reference: Telit LE910Cx AT Commands Reference Guide, Rev. 18
 */

#ifndef TELIT_LE910C4_H
#define TELIT_LE910C4_H

/* ══════════════════════════════════════════════════════════════════════════
 *  Standard 3GPP AT commands (common across most modems)
 * ══════════════════════════════════════════════════════════════════════════ */

/* General */
#define AT_CMD_ECHO_OFF             "ATE0"
#define AT_CMD_VERBOSE_ERRORS       "AT+CMEE=2"
#define AT_CMD_PING                 "AT"

/* Power management */
#define AT_CMD_CFUN_FULL            "AT+CFUN=1"
#define AT_CMD_CFUN_MINIMUM         "AT+CFUN=0"
#define AT_CMD_CFUN_RESET           "AT+CFUN=1,1"

/* SIM */
#define AT_CMD_QUERY_SIM            "AT+CPIN?"
/* AT+CPIN="<pin>" — built dynamically */

/* Network registration */
#define AT_CMD_QUERY_CREG           "AT+CREG?"
#define AT_CMD_QUERY_CEREG          "AT+CEREG?"       /* LTE/EPS */
#define AT_CMD_QUERY_SIGNAL         "AT+CSQ"
#define AT_CMD_QUERY_OPERATOR       "AT+COPS?"
#define AT_CMD_OPERATOR_FORMAT      "AT+COPS=3,0"     /* Long alphanumeric */

/* Identification */
#define AT_CMD_MANUFACTURER         "AT+CGMI"
#define AT_CMD_MODEL                "AT+CGMM"
#define AT_CMD_FIRMWARE             "AT+CGMR"
#define AT_CMD_IMEI                 "AT+CGSN"
#define AT_CMD_IMSI                 "AT+CIMI"

/* SMS */
#define AT_CMD_SMS_TEXT_MODE        "AT+CMGF=1"
#define AT_CMD_SMS_CHARSET          "AT+CSCS=\"GSM\""
#define AT_CMD_SMS_NOTIFY           "AT+CNMI=2,1,0,0,0"
#define AT_CMD_SMS_STORAGE          "AT+CPMS=\"ME\",\"ME\",\"ME\""
/* AT+CMGS="<number>" — built dynamically */
/* AT+CMGR=<index>    — built dynamically */
/* AT+CMGD=<index>    — built dynamically */
#define AT_CMD_SMS_DELETE_ALL       "AT+CMGD=1,4"

/* Voice call */
/* ATD<number>; — built dynamically */
#define AT_CMD_ANSWER               "ATA"
#define AT_CMD_HANGUP               "ATH"
#define AT_CMD_CALL_STATUS          "AT+CLCC"

/* DTMF */
/* AT+VTS=<tone>     — built dynamically */
/* AT+VTS="<string>" — built dynamically */
/* AT+VTD=<duration> — built dynamically */

/* Audio — basic (3GPP standard) */
#define AT_CMD_VOLUME               "AT+CLVL"         /* +<level> appended */
#define AT_CMD_MUTE_ON              "AT+CMUT=1"
#define AT_CMD_MUTE_OFF             "AT+CMUT=0"
#define AT_CMD_RINGER_VOLUME        "AT+CRSL"         /* +<level> appended */

/* ══════════════════════════════════════════════════════════════════════════
 *  Telit-proprietary AT commands (LE910C4 specific)
 * ══════════════════════════════════════════════════════════════════════════ */

#define AT_CMD_TELIT_ICCID          "AT#CCID"
#define AT_CMD_TELIT_USB_AUDIO_CFG  "AT#USBCFG=11"
#define AT_CMD_TELIT_HANGUP         "AT#CHUP"
/* AT#APLAY="<file>" — built dynamically */
/* AT#ASEND=<len>    — built dynamically */

/* Audio — Telit-proprietary (voice processing & codec) */
#define AT_CMD_TELIT_PROFILE_SEL    "AT#PSEL"         /* =<0|1> appended    */
#define AT_CMD_TELIT_CODECINFO      "AT#CODECINFO"
#define AT_CMD_TELIT_SPKMUT         "AT#SPKMUT"       /* =<0|1> appended    */
#define AT_CMD_TELIT_LOOPBACK       "AT#SRP"          /* =<0|2> appended    */
/* Handsfree voice processing */
#define AT_CMD_TELIT_HF_ECHO        "AT#SHFEC"        /* =<0|1>             */
#define AT_CMD_TELIT_HF_NR          "AT#SHFNR"        /* =<0|1>             */
#define AT_CMD_TELIT_HF_AGC         "AT#SHFAGC"       /* =<0|1>             */
#define AT_CMD_TELIT_HF_MICG        "AT#HFMICG"       /* =<0-7>             */
#define AT_CMD_TELIT_HF_SIDETONE    "AT#SHFSD"        /* =<0-15>            */
/* Handset voice processing */
#define AT_CMD_TELIT_HS_ECHO        "AT#SHSEC"        /* =<0|1>             */
#define AT_CMD_TELIT_HS_NR          "AT#SHSNR"        /* =<0|1>             */
#define AT_CMD_TELIT_HS_AGC         "AT#SHSAGC"       /* =<0|1>             */
#define AT_CMD_TELIT_HS_MICG        "AT#HSMICG"       /* =<0-7>             */
#define AT_CMD_TELIT_HS_SIDETONE    "AT#SHSSD"        /* =<0-15>            */
/* Tone volumes */
#define AT_CMD_TELIT_TSVOL          "AT#TSVOL"        /* =<class>,<0-5>     */

/* ══════════════════════════════════════════════════════════════════════════
 *  Timeouts (milliseconds)
 * ══════════════════════════════════════════════════════════════════════════ */

#define TELIT_TIMEOUT_DEFAULT       1500u
#define TELIT_TIMEOUT_SIM           5000u
#define TELIT_TIMEOUT_REG           3000u
#define TELIT_TIMEOUT_SMS_SEND      30000u
#define TELIT_TIMEOUT_SMS_READ      3000u
#define TELIT_TIMEOUT_DIAL          60000u
#define TELIT_TIMEOUT_CFUN          10000u
#define TELIT_TIMEOUT_IDENTITY      2000u
#define TELIT_TIMEOUT_AUDIO         5000u

/* ══════════════════════════════════════════════════════════════════════════
 *  Internal state machine for multi-step operations (e.g. SendSMS)
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    SMS_SEND_IDLE = 0,
    SMS_SEND_WAIT_PROMPT,       /* Waiting for ">" after AT+CMGS */
    SMS_SEND_WAIT_RESULT,       /* Waiting for OK after body + Ctrl-Z */
} SmsSendState;

typedef enum {
    SMS_CFG_IDLE = 0,
    SMS_CFG_TEXT_MODE,           /* AT+CMGF=1 */
    SMS_CFG_CHARSET,            /* AT+CSCS="GSM" */
    SMS_CFG_NOTIFY,             /* AT+CNMI=2,1,0,0,0 */
    SMS_CFG_STORAGE,            /* AT+CPMS="ME","ME","ME" */
} SmsCfgState;

typedef enum {
    NET_INFO_IDLE = 0,
    NET_INFO_REG,               /* AT+CREG? */
    NET_INFO_SIGNAL,            /* AT+CSQ */
    NET_INFO_OPERATOR,          /* AT+COPS? */
} NetInfoState;

typedef enum {
    IDENTITY_IDLE = 0,
    IDENTITY_MANUFACTURER,
    IDENTITY_MODEL,
    IDENTITY_FIRMWARE,
    IDENTITY_IMEI,
    IDENTITY_ICCID,
    IDENTITY_IMSI,
} IdentityState;

#endif /* TELIT_LE910C4_H */
