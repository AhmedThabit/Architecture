/**
 * @file    modem_api_types.h
 * @brief   Shared types for the modem abstraction layer.
 *
 * @details These enums and structs are used by modem_api.h (the abstract
 *          interface) and by every modem-specific backend implementation.
 *          They contain NO modem-specific or AT-command-specific content.
 */

#ifndef MODEM_API_TYPES_H
#define MODEM_API_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ══════════════════════════════════════════════════════════════════════════
 *  General result codes
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_OK = 0,               /**< Operation completed successfully       */
    MODEM_BUSY,                 /**< Engine busy with a previous operation   */
    MODEM_PENDING,              /**< Still in progress — poll again          */
    MODEM_TIMEOUT,              /**< No response within the deadline         */
    MODEM_ERROR,                /**< Modem returned ERROR                    */
    MODEM_ERROR_CME,            /**< +CME ERROR (network / SIM issue)        */
    MODEM_ERROR_CMS,            /**< +CMS ERROR (SMS issue)                  */
    MODEM_ERROR_PARAM,          /**< Invalid parameter passed by caller      */
    MODEM_ERROR_IO,             /**< UART / transport failure                */
    MODEM_NOT_READY,            /**< Modem not initialised or powered off    */
} Modem_Result;

/* ══════════════════════════════════════════════════════════════════════════
 *  SIM status
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_SIM_READY = 0,        /**< SIM is unlocked and ready              */
    MODEM_SIM_PIN_REQUIRED,     /**< SIM PIN is required                    */
    MODEM_SIM_PUK_REQUIRED,     /**< SIM PUK is required                    */
    MODEM_SIM_NOT_INSERTED,     /**< No SIM detected                        */
    MODEM_SIM_ERROR,            /**< SIM failure or unknown state            */
} Modem_SimStatus;

/* ══════════════════════════════════════════════════════════════════════════
 *  Network registration
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_NET_NOT_REGISTERED = 0, /**< Not registered, not searching        */
    MODEM_NET_HOME,               /**< Registered on home network           */
    MODEM_NET_SEARCHING,          /**< Searching for network                */
    MODEM_NET_DENIED,             /**< Registration denied                  */
    MODEM_NET_UNKNOWN,            /**< Unknown registration state           */
    MODEM_NET_ROAMING,            /**< Registered, roaming                  */
} Modem_NetStatus;

/* ══════════════════════════════════════════════════════════════════════════
 *  Network info (returned by Modem_GetNetworkInfo)
 * ══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    Modem_NetStatus  reg_status;        /**< Current registration status    */
    char             operator_name[32]; /**< Operator name (if available)   */
    int8_t           rssi_dbm;          /**< RSSI in dBm (-113 to -51)     */
    uint8_t          ber;               /**< Bit error rate (0-7, 99=N/A)  */
    uint8_t          csq;               /**< Raw CSQ value (0-31, 99=N/A)  */
} Modem_NetworkInfo;

/* ══════════════════════════════════════════════════════════════════════════
 *  Voice call status
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_CALL_IDLE = 0,        /**< No call in progress                    */
    MODEM_CALL_DIALING,         /**< Outgoing call, ringing remote side     */
    MODEM_CALL_ALERTING,        /**< Remote side is ringing                 */
    MODEM_CALL_ACTIVE,          /**< Call is connected                      */
    MODEM_CALL_ON_HOLD,         /**< Call is on hold                        */
    MODEM_CALL_INCOMING,        /**< Incoming call (RING)                   */
    MODEM_CALL_DISCONNECTED,    /**< Call was ended                         */
    MODEM_CALL_ERROR,           /**< Call setup failed                      */
} Modem_CallStatus;

/* ══════════════════════════════════════════════════════════════════════════
 *  SMS storage location (for read/delete)
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_SMS_STORAGE_SIM = 0,  /**< SIM card ("SM")                        */
    MODEM_SMS_STORAGE_MODEM,    /**< Modem internal memory ("ME")           */
} Modem_SmsStorage;

/* ══════════════════════════════════════════════════════════════════════════
 *  Incoming SMS structure (for Modem_ReadSMS)
 * ══════════════════════════════════════════════════════════════════════════ */

#define MODEM_SMS_SENDER_MAX    24
#define MODEM_SMS_BODY_MAX      160
#define MODEM_SMS_TIME_MAX      24

typedef struct {
    uint8_t     index;                          /**< SMS slot index         */
    char        sender[MODEM_SMS_SENDER_MAX];   /**< Sender phone number    */
    char        body[MODEM_SMS_BODY_MAX];       /**< Message text           */
    char        timestamp[MODEM_SMS_TIME_MAX];  /**< "YY/MM/DD,HH:MM:SS"   */
    bool        is_read;                        /**< Already-read flag      */
} Modem_SmsMessage;

/* ══════════════════════════════════════════════════════════════════════════
 *  Power mode
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    MODEM_POWER_MINIMUM = 0,    /**< Airplane / minimum functionality       */
    MODEM_POWER_FULL,           /**< Full online operation                  */
    MODEM_POWER_RESET,          /**< Reset the modem module                 */
} Modem_PowerMode;

/* ══════════════════════════════════════════════════════════════════════════
 *  Modem identification (returned by Modem_GetIdentity)
 * ══════════════════════════════════════════════════════════════════════════ */

typedef struct {
    char manufacturer[32];      /**< e.g. "Telit"                           */
    char model[32];             /**< e.g. "LE910C4-EU"                      */
    char firmware[48];          /**< Firmware version string                 */
    char imei[20];              /**< 15-digit IMEI                          */
    char iccid[24];             /**< SIM ICCID (up to 20 digits)            */
    char imsi[20];              /**< SIM IMSI                               */
} Modem_Identity;

/* ══════════════════════════════════════════════════════════════════════════
 *  URC callback types (unsolicited result codes)
 * ══════════════════════════════════════════════════════════════════════════ */

/** Called when a RING / incoming call is detected. */
typedef void (*Modem_RingCallback)(const char *caller_id);

/** Called when a new SMS arrives (+CMTI notification). */
typedef void (*Modem_SmsReceivedCallback)(uint8_t index, Modem_SmsStorage storage);

/** Called when call status changes (NO CARRIER, BUSY, etc.). */
typedef void (*Modem_CallStatusCallback)(Modem_CallStatus new_status);

/* ══════════════════════════════════════════════════════════════════════════
 *  Audio path and codec configuration types
 * ══════════════════════════════════════════════════════════════════════════ */

/** Audio path selection — where voice audio is routed. */
typedef enum {
    MODEM_AUDIO_PATH_HANDSET = 0,   /**< External codec on Aux-PCM (MAX9867)*/
    MODEM_AUDIO_PATH_HEADSET,       /**< Headset jack (if available)        */
    MODEM_AUDIO_PATH_SPEAKER,       /**< Handsfree / loudspeaker            */
    MODEM_AUDIO_PATH_USB,           /**< USB audio class (#USBCFG=11)       */
} Modem_AudioPath;

/** Audio profile — selects echo-canceller & gain tuning set. */
typedef enum {
    MODEM_AUDIO_PROFILE_HANDSET = 0,  /**< Close-talk profile               */
    MODEM_AUDIO_PROFILE_HANDSFREE,    /**< Far-field / loudspeaker profile   */
} Modem_AudioProfile;

/** Codec clock source — determines MAX9867 MCLK origin. */
typedef enum {
    MODEM_CODEC_CLK_MODEM = 0,      /**< MCLK from modem PCM_CLK output    */
    MODEM_CODEC_CLK_CRYSTAL,        /**< External crystal / oscillator      */
} Modem_CodecClkSource;

/** Audio codec status (returned by Audio_QueryCodec). */
typedef struct {
    uint8_t     codec_type;         /**< 0=none, 1=MAX9867, etc.            */
    uint8_t     sample_rate_khz;    /**< 8 or 16                            */
    uint8_t     bit_depth;          /**< 16                                 */
    bool        active;             /**< true if codec is streaming         */
} Modem_CodecInfo;

/** Full audio configuration for multi-step setup. */
typedef struct {
    Modem_AudioPath         path;       /**< Audio routing selection         */
    Modem_AudioProfile      profile;    /**< Echo-cancel / gain profile      */
    Modem_CodecClkSource    clk_src;    /**< Clock source for codec          */
    uint8_t                 spk_vol;    /**< Speaker volume (0–100)          */
    uint8_t                 mic_gain;   /**< Microphone gain (0–100)         */
    bool                    ec_enable;  /**< Echo canceller on/off           */
    bool                    nr_enable;  /**< Noise reduction on/off          */
    bool                    agc_enable; /**< Auto gain control on/off        */
} Modem_AudioConfig;

#endif /* MODEM_API_TYPES_H */
