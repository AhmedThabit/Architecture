/**
 * @file    modem_api.h
 * @brief   Abstract modem interface — modem-brand-agnostic API.
 *
 * @details This header defines the COMPLETE set of operations available on
 *          a cellular modem.  Application code (protothreads, services)
 *          includes ONLY this file — never modem-specific headers.
 *
 *          ┌─────────────────────────────────────────────────────────┐
 *          │  To swap modem hardware (e.g. Telit → Quectel → 5G):  │
 *          │    1. Create a new drivers/modem/xxx_backend.c         │
 *          │    2. Implement every function declared here            │
 *          │    3. Update the #include in modem_api.c                │
 *          │    4. App code and protothreads remain UNCHANGED        │
 *          └─────────────────────────────────────────────────────────┘
 *
 *          Every operation follows the Cmd / Poll pattern for use with
 *          Protothreads:
 *
 *            Modem_Xxx_Cmd(params);
 *            PT_WAIT_UNTIL(pt, Modem_Xxx_Poll(&result));
 *
 *          The _Cmd function starts the operation.
 *          The _Poll function returns false while pending, true when done.
 *          The result parameter receives MODEM_OK or an error code.
 *
 * @note    For simple operations (echo off, ping) where only OK/ERROR is
 *          expected, use Modem_SimpleCmd() + Modem_SimplePoll().
 *
 * @note    Reference commands for Telit LE910C4:
 *          - Telit LE910Cx AT Commands Reference Guide, Rev. 18 (2023)
 *          - 3GPP TS 27.007 (General AT commands)
 *          - 3GPP TS 27.005 (SMS AT commands)
 */

#ifndef MODEM_API_H
#define MODEM_API_H

#include "modem_api_types.h"

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  1. INITIALISATION & LIFECYCLE                                       ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Initialise the modem subsystem (UART, AT engine, internal state).
 *         Call once at startup after SYS_Initialize().
 */
void Modem_Init(void);

/**
 * @brief  Set modem power mode (full, minimum, reset).
 *         Cmd/Poll: Modem_SetPowerMode_Cmd() → Modem_SetPowerMode_Poll()
 *
 *         AT+CFUN=<mode>
 */
void Modem_SetPowerMode_Cmd(Modem_PowerMode mode);
bool Modem_SetPowerMode_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  2. PREFLIGHT / CONFIGURATION                                        ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Disable command echo.
 *         ATE0
 */
void Modem_EchoOff_Cmd(void);
bool Modem_EchoOff_Poll(Modem_Result *result);

/**
 * @brief  Enable verbose error reporting.
 *         AT+CMEE=2
 */
void Modem_VerboseErrors_Cmd(void);
bool Modem_VerboseErrors_Poll(Modem_Result *result);

/**
 * @brief  Ping the modem ("AT").
 */
void Modem_Ping_Cmd(void);
bool Modem_Ping_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  3. SIM CARD                                                          ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Query SIM card status.
 *         AT+CPIN?
 */
void Modem_QuerySIM_Cmd(void);
bool Modem_QuerySIM_Poll(Modem_Result *result, Modem_SimStatus *status);

/**
 * @brief  Enter SIM PIN code.
 *         AT+CPIN="<pin>"
 */
void Modem_EnterPIN_Cmd(const char *pin);
bool Modem_EnterPIN_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  4. NETWORK REGISTRATION                                             ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Query network registration status.
 *         AT+CREG?  /  AT+CEREG?  (modem-specific)
 */
void Modem_QueryRegistration_Cmd(void);
bool Modem_QueryRegistration_Poll(Modem_Result *result, Modem_NetStatus *status);

/**
 * @brief  Query signal quality (RSSI, BER).
 *         AT+CSQ  /  AT+CESQ  (modem-specific)
 */
void Modem_QuerySignal_Cmd(void);
bool Modem_QuerySignal_Poll(Modem_Result *result, int8_t *rssi_dbm, uint8_t *ber);

/**
 * @brief  Query current network operator name.
 *         AT+COPS?
 */
void Modem_QueryOperator_Cmd(void);
bool Modem_QueryOperator_Poll(Modem_Result *result, char *name, size_t name_max);

/**
 * @brief  Get a comprehensive network info snapshot in one call.
 *         (Internally may issue AT+CREG? + AT+CSQ + AT+COPS?)
 */
void Modem_GetNetworkInfo_Cmd(void);
bool Modem_GetNetworkInfo_Poll(Modem_Result *result, Modem_NetworkInfo *info);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  5. MODEM IDENTIFICATION                                             ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Query modem manufacturer, model, firmware, IMEI, ICCID, IMSI.
 *         Internally issues: ATI, AT+CGMI, AT+CGMM, AT+CGMR,
 *                            AT+CGSN, AT+CCID, AT+CIMI
 */
void Modem_GetIdentity_Cmd(void);
bool Modem_GetIdentity_Poll(Modem_Result *result, Modem_Identity *id);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  6. SMS — SEND                                                        ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Configure SMS subsystem (text mode, charset, storage, notifications).
 *         Internally issues:  AT+CMGF=1, AT+CSCS="GSM", AT+CNMI=...,
 *                             AT+CPMS="ME","ME","ME"
 */
void Modem_ConfigSMS_Cmd(void);
bool Modem_ConfigSMS_Poll(Modem_Result *result);

/**
 * @brief  Send an SMS message (complete operation: AT+CMGS + body + Ctrl-Z).
 *
 * @param  number  Destination phone number (international format "+20...")
 * @param  text    Message body (max 160 chars for GSM 7-bit encoding)
 *
 * @note   This is a SINGLE Cmd/Poll pair.  The multi-step AT dance
 *         (CMGS → ">" prompt → body → Ctrl-Z → OK) is handled internally
 *         by the backend.  The application never sees the intermediate steps.
 */
void Modem_SendSMS_Cmd(const char *number, const char *text);
bool Modem_SendSMS_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  7. SMS — RECEIVE / READ / DELETE                                     ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Read an SMS by index.
 *         AT+CMGR=<index>
 */
void Modem_ReadSMS_Cmd(uint8_t index);
bool Modem_ReadSMS_Poll(Modem_Result *result, Modem_SmsMessage *msg);

/**
 * @brief  Delete an SMS by index.
 *         AT+CMGD=<index>
 */
void Modem_DeleteSMS_Cmd(uint8_t index);
bool Modem_DeleteSMS_Poll(Modem_Result *result);

/**
 * @brief  Delete all SMS messages.
 *         AT+CMGD=1,4
 */
void Modem_DeleteAllSMS_Cmd(void);
bool Modem_DeleteAllSMS_Poll(Modem_Result *result);

/**
 * @brief  Register a callback for incoming SMS notifications (+CMTI URC).
 *
 * @param  cb  Function to call when a new SMS arrives (NULL to unregister)
 */
void Modem_OnSmsReceived(Modem_SmsReceivedCallback cb);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  8. VOICE CALL                                                        ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Initiate a voice call.
 *         ATD<number>;
 *
 * @param  number  Destination phone number
 */
void Modem_Dial_Cmd(const char *number);
bool Modem_Dial_Poll(Modem_Result *result);

/**
 * @brief  Answer an incoming call.
 *         ATA
 */
void Modem_Answer_Cmd(void);
bool Modem_Answer_Poll(Modem_Result *result);

/**
 * @brief  Hang up the current call.
 *         ATH  /  AT#CHUP  (modem-specific)
 */
void Modem_Hangup_Cmd(void);
bool Modem_Hangup_Poll(Modem_Result *result);

/**
 * @brief  Query current call status.
 *         AT+CLCC
 */
void Modem_QueryCallStatus_Cmd(void);
bool Modem_QueryCallStatus_Poll(Modem_Result *result, Modem_CallStatus *status);

/**
 * @brief  Register a callback for RING / call status changes.
 *
 * @param  ring_cb   Called on incoming call (NULL to unregister)
 * @param  status_cb Called on call status changes (NULL to unregister)
 */
void Modem_OnRing(Modem_RingCallback ring_cb);
void Modem_OnCallStatus(Modem_CallStatusCallback status_cb);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  9. DTMF                                                             ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Send a DTMF tone during an active call.
 *         AT+VTS=<tone>
 *
 * @param  tone     DTMF character: '0'-'9', '*', '#', 'A'-'D'
 */
void Modem_SendDTMF_Cmd(char tone);
bool Modem_SendDTMF_Poll(Modem_Result *result);

/**
 * @brief  Send a DTMF string (multiple tones in sequence).
 *         AT+VTS="<string>"
 *
 * @param  tones    String of DTMF characters (e.g. "1234#")
 */
void Modem_SendDTMFString_Cmd(const char *tones);
bool Modem_SendDTMFString_Poll(Modem_Result *result);

/**
 * @brief  Set DTMF tone duration.
 *         AT+VTD=<duration>
 *
 * @param  duration_tenths  Duration in 1/10 seconds (0 = network default)
 */
void Modem_SetDTMFDuration_Cmd(uint8_t duration_tenths);
bool Modem_SetDTMFDuration_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  10. AUDIO (modem-specific, may be unavailable on some backends)     ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Configure audio routing (e.g. USB audio for Telit LE910C4).
 *         AT#USBCFG=11  (Telit-specific)
 */
void Modem_ConfigAudio_Cmd(void);
bool Modem_ConfigAudio_Poll(Modem_Result *result);

/**
 * @brief  Set audio volume level.
 *         AT+CLVL=<level>
 *
 * @param  level  Volume level (0-100, mapped to modem range internally)
 */
void Modem_SetVolume_Cmd(uint8_t level);
bool Modem_SetVolume_Poll(Modem_Result *result);

/**
 * @brief  Play an audio file stored on the modem's filesystem.
 *         AT#APLAY="<filename>"  (Telit-specific)
 *
 * @param  filename  Path on modem filesystem
 *
 * @note   Returns MODEM_ERROR on modems that don't support audio playback.
 */
void Modem_PlayAudio_Cmd(const char *filename);
bool Modem_PlayAudio_Poll(Modem_Result *result);

/**
 * @brief  Send raw PCM audio data to the modem during a call.
 *         AT#ASEND=<len>  (Telit-specific)
 *
 * @param  data  PCM audio samples
 * @param  len   Number of bytes
 *
 * @note   Returns MODEM_ERROR on modems that don't support raw audio send.
 */
void Modem_SendAudio_Cmd(const uint8_t *data, size_t len);
bool Modem_SendAudio_Poll(Modem_Result *result);

/**
 * @brief  Mute/unmute the microphone.
 *         AT+CMUT=<mode>
 *
 * @param  mute  true = mute, false = unmute
 */
void Modem_SetMute_Cmd(bool mute);
bool Modem_SetMute_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  11. POWER MANAGEMENT                                                ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Query modem firmware version string.
 *         AT+CGMR
 */
void Modem_QueryFirmwareVersion_Cmd(void);
bool Modem_QueryFirmwareVersion_Poll(Modem_Result *result, char *ver, size_t ver_max);

/**
 * @brief  Query IMEI.
 *         AT+CGSN
 */
void Modem_QueryIMEI_Cmd(void);
bool Modem_QueryIMEI_Poll(Modem_Result *result, char *imei, size_t imei_max);

/**
 * @brief  Query SIM ICCID.
 *         AT+CCID  /  AT#CCID  (modem-specific)
 */
void Modem_QueryICCID_Cmd(void);
bool Modem_QueryICCID_Poll(Modem_Result *result, char *iccid, size_t iccid_max);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  12. URC PROCESSING (call from main loop)                            ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Process any pending unsolicited result codes (URCs).
 *         Call this periodically from the main loop or a protothread.
 *         It checks the UART for RING, +CMTI, NO CARRIER, etc. and
 *         dispatches the registered callbacks.
 */
void Modem_ProcessURCs(void);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  13. GENERIC / RAW AT (escape hatch)                                 ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Send a raw AT command string and wait for OK/ERROR.
 *         Use this for modem-specific commands not covered by the API.
 *
 * @param  cmd         Full AT command string (e.g. "AT+COPS=0")
 * @param  timeout_ms  Maximum wait time
 */
void Modem_RawCmd_Cmd(const char *cmd, uint32_t timeout_ms);
bool Modem_RawCmd_Poll(Modem_Result *result);

/**
 * @brief  Get the raw accumulator contents after a RawCmd.
 *         Useful for parsing custom responses.
 * @return Pointer to internal null-terminated buffer (read-only).
 */
const char *Modem_GetRawResponse(void);

#endif /* MODEM_API_H */
