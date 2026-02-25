/**
 * @file    gsm_service.h
 * @brief   Tier 3 — Modem-specific GSM service layer.
 *
 * @details High-level API for the emergency dialer application.
 *          Each function maps to a specific use-case: dial, SMS, signal, etc.
 *
 *          This file contains the AT command strings for the **Telit LE910C4**.
 *          To switch to a different modem (e.g. 5G Telit FN990A28 or
 *          Quectel EG25), you replace/modify THIS file only.
 *          Tiers 1 (modem_uart) and 2 (modem_at) remain unchanged.
 *
 *          All functions are designed to be called from Protothreads using
 *          PT_WAIT_UNTIL with the polling-style AT engine.
 *
 * @note    References:
 *            - Telit LE910Cx AT Commands Reference Guide
 *            - 3GPP TS 27.005 (SMS AT commands)
 *            - 3GPP TS 27.007 (General AT commands)
 */

#ifndef GSM_SERVICE_H
#define GSM_SERVICE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Result codes ───────────────────────────────────────────────────────── */

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
    GSM_ERR_CME,
    GSM_ERR_CMS,
} GSM_Result;

/* ── Network registration status ────────────────────────────────────────── */

typedef enum {
    GSM_NET_NOT_REGISTERED = 0,
    GSM_NET_HOME           = 1,
    GSM_NET_SEARCHING      = 2,
    GSM_NET_DENIED         = 3,
    GSM_NET_UNKNOWN        = 4,
    GSM_NET_ROAMING        = 5,
} GSM_NetStatus;

/* ── SIM status ─────────────────────────────────────────────────────────── */

typedef enum {
    GSM_SIM_READY   = 0,
    GSM_SIM_PIN     = 1,
    GSM_SIM_PUK     = 2,
    GSM_SIM_OTHER   = 3,
    GSM_SIM_UNKNOWN = -1,
} GSM_SimStatus;

/* ── Initialisation & preflight ─────────────────────────────────────────── */

/**
 * @brief  Initialise the GSM service and underlying modem layers.
 *         Call once at startup after SYS_Initialize().
 */
void GSM_Service_Init(void);

/* ── Preflight AT commands (send one, then poll for response) ───────────── */

/** Send "ATE0" to disable echo. */
void GSM_Cmd_EchoOff(void);

/** Send "AT+CMEE=2" for verbose error codes. */
void GSM_Cmd_VerboseErrors(void);

/** Send "AT+CPIN?" to query SIM status. */
void GSM_Cmd_QuerySIM(void);

/**
 * @brief  Poll for +CPIN response after GSM_Cmd_QuerySIM().
 * @param  status  receives the SIM status
 * @return true when response received or timeout elapsed
 */
bool GSM_Poll_SIMStatus(GSM_SimStatus *status);

/** Send "AT+CREG?" to query network registration. */
void GSM_Cmd_QueryRegistration(void);

/**
 * @brief  Poll for +CREG response after GSM_Cmd_QueryRegistration().
 * @param  registered  true if registered (home or roaming)
 * @return true when response received or timeout elapsed
 */
bool GSM_Poll_Registration(bool *registered);

/** Send commands to configure SMS text mode (AT+CMGF=1, AT+CSCS, etc). */
void GSM_Cmd_ConfigSMS(void);

/**
 * @brief  Poll for OK/ERROR after GSM_Cmd_ConfigSMS().
 * @param  which  0=OK, 1=ERROR, -1=timeout
 * @return true when complete
 */
bool GSM_Poll_ConfigSMS(int *which);

/* ── SMS operations ─────────────────────────────────────────────────────── */

/**
 * @brief  Begin sending an SMS: issues AT+CMGS="number".
 * @param  number  destination phone number (international format)
 */
void GSM_Cmd_BeginSMS(const char *number);

/**
 * @brief  Poll for the ">" prompt after GSM_Cmd_BeginSMS().
 * @param  which  0=">", 1="CONNECT", 2="ERROR", -1=timeout
 * @return true when prompt/error received or timeout
 */
bool GSM_Poll_SMSPrompt(int *which);

/**
 * @brief  Send SMS body text followed by Ctrl-Z to submit.
 * @param  text  message body
 */
void GSM_Cmd_SendSMSBody(const char *text);

/**
 * @brief  Poll for +CMGS / OK after submitting SMS body.
 * @param  which  0=OK, 1=ERROR, -1=timeout
 * @return true when delivery confirmed or timeout
 */
bool GSM_Poll_SMSResult(int *which);

/* ── Voice call ─────────────────────────────────────────────────────────── */

/**
 * @brief  Initiate a voice call: ATD<number>;
 * @param  number  destination phone number
 */
void GSM_Cmd_Dial(const char *number);

/**
 * @brief  Hang up: ATH
 */
void GSM_Cmd_Hangup(void);

/* ── Signal quality ─────────────────────────────────────────────────────── */

/** Send "AT+CSQ" */
void GSM_Cmd_QuerySignal(void);

/**
 * @brief  Poll for +CSQ response.
 * @param  rssi_dbm  approximate dBm (converted from CSQ value)
 * @return true when response received or timeout
 */
bool GSM_Poll_Signal(int8_t *rssi_dbm);

/* ── USB Audio (LE910C4 Linux-specific) ─────────────────────────────────── */

#if defined(CFG_MODEM_TELIT_LE910C4)

/**
 * @brief  Configure USB audio mode (AT#USBCFG=11).
 */
void GSM_Cmd_ConfigUSBAudio(void);

/**
 * @brief  Play an audio file over the voice call (AT#APLAY).
 * @param  filename  path on the modem's filesystem
 */
void GSM_Cmd_PlayAudio(const char *filename);

/**
 * @brief  Send raw audio data (AT#ASEND).
 * @param  data  PCM audio samples
 * @param  len   number of bytes
 */
void GSM_Cmd_SendAudio(const uint8_t *data, size_t len);

#endif /* CFG_MODEM_TELIT_LE910C4 */

/* ── Generic AT (for ping / keep-alive) ─────────────────────────────────── */

/** Send "AT" and begin exchange with specified timeout. */
void GSM_Cmd_Ping(uint32_t timeout_ms);

/**
 * @brief  Poll for OK/ERROR after any simple command.
 * @param  which  0=OK, 1=ERROR, -1=timeout
 * @return true when complete
 */
bool GSM_Poll_OkError(int *which);

#endif /* GSM_SERVICE_H */
