/**
 * @file    telit_audio.c
 * @brief   Telit LE910C4 audio backend — MAX9867 codec control via AT commands.
 *
 * @details The LE910C4 has an internal Qualcomm MDM9628 SoC running Linux.
 *          It communicates with the external MAX9867 codec via:
 *            - I2C bus  → register configuration (clocking, gain, filters)
 *            - Aux PCM  → audio data streaming (voice samples)
 *
 *          The modem's ALSA/ASoC audio subsystem handles ALL codec I2C
 *          register writes and PCM streaming.  The PIC32MM firmware controls
 *          the audio system ONLY through AT commands over UART.
 *
 *          Hardware:
 *            ┌──────────┐  I2C + PCM  ┌──────────┐
 *            │ LE910C4  │────────────→│ MAX9867  │──→ Speaker / Mic
 *            └────┬─────┘             └──────────┘
 *                 │ UART3 (AT cmds)
 *            ┌────┴─────┐
 *            │ PIC32MM  │   ← Controls audio via AT commands ONLY
 *            └──────────┘
 *
 *          MAX9867 clock configuration:
 *            - If MCLK from modem PCM_CLK → modem sets PSCLK + NI auto
 *            - If MCLK from external crystal → modem configures NI divider
 *              based on the crystal frequency the calibration expects
 *
 *          Audio profile selection (#PSEL) chooses calibration data set:
 *          echo cancellation, noise reduction, AGC, gain curves, EQ.
 */

#include "services/audio/audio_api.h"
#include "drivers/modem/modem_at.h"
#include "drivers/modem/modem_uart.h"
#include "config/app_config.h"
#include <string.h>
#include <stdio.h>

/* ══════════════════════════════════════════════════════════════════════════
 *  AT Command Format Strings (Telit LE910C4 specific)
 * ══════════════════════════════════════════════════════════════════════════ */

/* Audio profile selection */
#define AT_PSEL_FMT             "AT#PSEL=%u"        /* 0=handset, 1=HF      */

/* Speaker volume */
#define AT_CLVL_SET_FMT         "AT+CLVL=%u"        /* 0–5                  */
#define AT_CLVL_QUERY           "AT+CLVL?"

/* Microphone gain */
#define AT_HFMICG_FMT           "AT#HFMICG=%u"      /* Handsfree mic 0–7    */
#define AT_HSMICG_FMT           "AT#HSMICG=%u"      /* Handset mic 0–7      */

/* Mute */
#define AT_CMUT_FMT             "AT+CMUT=%u"         /* 0=unmute, 1=mute     */
#define AT_SPKMUT_FMT           "AT#SPKMUT=%u"       /* Speaker mute         */

/* Echo cancellation */
#define AT_SHFEC_FMT            "AT#SHFEC=%u"        /* HF echo cancel       */
#define AT_SHSEC_FMT            "AT#SHSEC=%u"        /* HS echo cancel       */

/* Noise reduction */
#define AT_SHFNR_FMT            "AT#SHFNR=%u"        /* HF noise reduction   */
#define AT_SHSNR_FMT            "AT#SHSNR=%u"        /* HS noise reduction   */

/* Automatic gain control */
#define AT_SHFAGC_FMT           "AT#SHFAGC=%u"       /* HF AGC               */
#define AT_SHSAGC_FMT           "AT#SHSAGC=%u"       /* HS AGC               */

/* Sidetone */
#define AT_SHFSD_FMT            "AT#SHFSD=%u"        /* HF sidetone 0–15     */
#define AT_SHSSD_FMT            "AT#SHSSD=%u"        /* HS sidetone 0–15     */

/* Codec info */
#define AT_CODECINFO            "AT#CODECINFO"

/* Audio loopback */
#define AT_SRP_FMT              "AT#SRP=%u"          /* 0=normal, 2=loopback */

/* Ringer volume */
#define AT_CRSL_FMT             "AT+CRSL=%u"         /* 0–100                */

/* Tone class volume */
#define AT_TSVOL_FMT            "AT#TSVOL=%u,%u"     /* class, level 0–5     */

/* USB audio composition */
#define AT_USBCFG_FMT           "AT#USBCFG=%u"

/* ══════════════════════════════════════════════════════════════════════════
 *  Timeouts (milliseconds)
 * ══════════════════════════════════════════════════════════════════════════ */

#define AUDIO_TIMEOUT_DEFAULT       1500u
#define AUDIO_TIMEOUT_PROFILE       3000u   /* Profile switch may take time  */
#define AUDIO_TIMEOUT_USBCFG       10000u   /* USB reconfig triggers reboot  */

/* ══════════════════════════════════════════════════════════════════════════
 *  Internal state machine for Audio_Init (multi-step)
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    AUDIO_INIT_IDLE = 0,
    AUDIO_INIT_PROFILE,         /* AT#PSEL                                  */
    AUDIO_INIT_VOLUME,          /* AT+CLVL                                  */
    AUDIO_INIT_MIC_GAIN,        /* AT#HFMICG or AT#HSMICG                   */
    AUDIO_INIT_ECHO_CANCEL,     /* AT#SHFEC or AT#SHSEC                     */
    AUDIO_INIT_NOISE_REDUCE,    /* AT#SHFNR or AT#SHSNR                     */
    AUDIO_INIT_AGC,             /* AT#SHFAGC or AT#SHSAGC                   */
    AUDIO_INIT_CODEC_QUERY,     /* AT#CODECINFO (verify)                    */
} AudioInitState;

/* ══════════════════════════════════════════════════════════════════════════
 *  Module-level state
 * ══════════════════════════════════════════════════════════════════════════ */

static AudioInitState       s_init_state = AUDIO_INIT_IDLE;
static Modem_AudioConfig    s_active_cfg;        /* Current configuration    */
static Modem_AudioProfile   s_active_profile = MODEM_AUDIO_PROFILE_HANDSET;
static char                 s_cmd_buf[48];       /* AT command scratch buffer*/

/* ══════════════════════════════════════════════════════════════════════════
 *  Default configuration (from app_config.h)
 * ══════════════════════════════════════════════════════════════════════════ */

static const Modem_AudioConfig s_default_config = {
    .path       = CFG_AUDIO_DEFAULT_PATH,
    .profile    = CFG_AUDIO_DEFAULT_PROFILE,
    .clk_src    = CFG_AUDIO_CLK_SOURCE,
    .spk_vol    = CFG_AUDIO_DEFAULT_SPK_VOL,
    .mic_gain   = CFG_AUDIO_DEFAULT_MIC_GAIN,
    .ec_enable  = CFG_AUDIO_ECHO_CANCEL,
    .nr_enable  = CFG_AUDIO_NOISE_REDUCE,
    .agc_enable = CFG_AUDIO_AGC_ENABLE,
};

/* ══════════════════════════════════════════════════════════════════════════
 *  Helper: map 0–100 percentage to Telit range
 * ══════════════════════════════════════════════════════════════════════════ */

/** Map 0–100 → 0–max (integer, rounding). */
static uint8_t map_range(uint8_t val_100, uint8_t max_val)
{
    if (val_100 >= 100) return max_val;
    return (uint8_t)(((uint16_t)val_100 * max_val + 50u) / 100u);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Helper: generic OK/ERROR poll  (matches telit_le910c4.c pattern)
 * ══════════════════════════════════════════════════════════════════════════ */

/**
 * Uses the AT engine's WaitAny to look for "OK" or "ERROR".
 * Returns true when the exchange is complete (OK, ERROR, or timeout).
 */
static bool poll_ok_error(Modem_Result *result)
{
    static const char *tokens[] = { "OK", "ERROR" };
    int which = -1;
    bool done = Modem_AT_WaitAny(tokens, 2, &which);
    if (done && result) {
        if (which == 0)      *result = MODEM_OK;
        else if (which == 1) *result = MODEM_ERROR;
        else                 *result = MODEM_TIMEOUT;
    }
    return done;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Helper: send next init step
 * ══════════════════════════════════════════════════════════════════════════ */

static void send_init_step(void)
{
    switch (s_init_state) {

    case AUDIO_INIT_PROFILE:
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_PSEL_FMT,
                 (unsigned)(s_active_cfg.profile == MODEM_AUDIO_PROFILE_HANDSFREE ? 1u : 0u));
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_PROFILE);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_VOLUME:
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_CLVL_SET_FMT,
                 (unsigned)map_range(s_active_cfg.spk_vol, 5));
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_MIC_GAIN:
        {
            uint8_t mic_val = map_range(s_active_cfg.mic_gain, 7);
            if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
                snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_HFMICG_FMT, (unsigned)mic_val);
            } else {
                snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_HSMICG_FMT, (unsigned)mic_val);
            }
        }
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_ECHO_CANCEL:
        if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFEC_FMT,
                     (unsigned)(s_active_cfg.ec_enable ? 1u : 0u));
        } else {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSEC_FMT,
                     (unsigned)(s_active_cfg.ec_enable ? 1u : 0u));
        }
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_NOISE_REDUCE:
        if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFNR_FMT,
                     (unsigned)(s_active_cfg.nr_enable ? 1u : 0u));
        } else {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSNR_FMT,
                     (unsigned)(s_active_cfg.nr_enable ? 1u : 0u));
        }
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_AGC:
        if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFAGC_FMT,
                     (unsigned)(s_active_cfg.agc_enable ? 1u : 0u));
        } else {
            snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSAGC_FMT,
                     (unsigned)(s_active_cfg.agc_enable ? 1u : 0u));
        }
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(s_cmd_buf);
        break;

    case AUDIO_INIT_CODEC_QUERY:
        Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
        Modem_AT_SendCmd(AT_CODECINFO);
        break;

    default:
        break;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  1. Audio Init (multi-step)
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_Init_Cmd(const Modem_AudioConfig *config)
{
    /* Use provided config or default */
    if (config != NULL) {
        s_active_cfg = *config;
    } else {
        s_active_cfg = s_default_config;
    }

    s_active_profile = s_active_cfg.profile;

    /* Start with profile selection */
    s_init_state = AUDIO_INIT_PROFILE;
    send_init_step();
}

bool Audio_Init_Poll(Modem_Result *result)
{
    Modem_Result step_res;

    if (s_init_state == AUDIO_INIT_IDLE) {
        *result = MODEM_OK;
        return true;
    }

    /* Wait for current step to complete */
    if (!poll_ok_error(&step_res)) {
        return false;
    }

    /* If step failed (except CODEC_QUERY which is advisory) → abort */
    if (step_res != MODEM_OK && s_init_state != AUDIO_INIT_CODEC_QUERY) {
        *result = step_res;
        s_init_state = AUDIO_INIT_IDLE;
        return true;
    }

    /* Advance to next step */
    switch (s_init_state) {
        case AUDIO_INIT_PROFILE:      s_init_state = AUDIO_INIT_VOLUME;       break;
        case AUDIO_INIT_VOLUME:       s_init_state = AUDIO_INIT_MIC_GAIN;     break;
        case AUDIO_INIT_MIC_GAIN:     s_init_state = AUDIO_INIT_ECHO_CANCEL;  break;
        case AUDIO_INIT_ECHO_CANCEL:  s_init_state = AUDIO_INIT_NOISE_REDUCE; break;
        case AUDIO_INIT_NOISE_REDUCE: s_init_state = AUDIO_INIT_AGC;          break;
        case AUDIO_INIT_AGC:          s_init_state = AUDIO_INIT_CODEC_QUERY;  break;
        case AUDIO_INIT_CODEC_QUERY:
            /* Done — all steps completed */
            *result = MODEM_OK;
            s_init_state = AUDIO_INIT_IDLE;
            return true;
        default:
            *result = MODEM_ERROR;
            s_init_state = AUDIO_INIT_IDLE;
            return true;
    }

    /* Send the next step's AT command */
    send_init_step();
    return false;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  2. Profile selection
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SelectProfile_Cmd(Modem_AudioProfile profile)
{
    s_active_profile = profile;
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_PSEL_FMT,
             (unsigned)(profile == MODEM_AUDIO_PROFILE_HANDSFREE ? 1u : 0u));
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_PROFILE);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SelectProfile_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  3. Speaker volume
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetVolume_Cmd(uint8_t level)
{
    uint8_t modem_level = map_range(level, 5);
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_CLVL_SET_FMT, (unsigned)modem_level);
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetVolume_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Audio_GetVolume_Cmd(void)
{
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CLVL_QUERY);
}

bool Audio_GetVolume_Poll(Modem_Result *result, uint8_t *level)
{
    if (!poll_ok_error(result)) return false;

    if (*result == MODEM_OK && level != NULL) {
        /* Parse "+CLVL: <level>" from accumulator */
        const char *acc = Modem_AT_GetAccumulator();
        const char *p = strstr(acc, "+CLVL:");
        if (p != NULL) {
            unsigned val = 0;
            p += 6; /* skip "+CLVL:" */
            while (*p == ' ') p++;
            while (*p >= '0' && *p <= '9') {
                val = val * 10 + (*p - '0');
                p++;
            }
            /* Map 0–5 back to 0–100 */
            if (val > 5) val = 5;
            *level = (uint8_t)((val * 100u + 2u) / 5u);
        } else {
            *level = 0;
        }
    }
    return true;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  4. Microphone gain
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetMicGain_Cmd(uint8_t gain)
{
    uint8_t modem_gain = map_range(gain, 7);
    if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_HFMICG_FMT, (unsigned)modem_gain);
    } else {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_HSMICG_FMT, (unsigned)modem_gain);
    }
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetMicGain_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  5. Mute control
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetMute_Cmd(bool mute)
{
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_CMUT_FMT, (unsigned)(mute ? 1u : 0u));
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetMute_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Audio_SetSpeakerMute_Cmd(bool mute)
{
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SPKMUT_FMT, (unsigned)(mute ? 1u : 0u));
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetSpeakerMute_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  6. Echo cancellation & noise reduction
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetEchoCanceller_Cmd(bool enable)
{
    if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFEC_FMT, (unsigned)(enable ? 1u : 0u));
    } else {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSEC_FMT, (unsigned)(enable ? 1u : 0u));
    }
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetEchoCanceller_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Audio_SetNoiseReduction_Cmd(bool enable)
{
    if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFNR_FMT, (unsigned)(enable ? 1u : 0u));
    } else {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSNR_FMT, (unsigned)(enable ? 1u : 0u));
    }
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetNoiseReduction_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Audio_SetAGC_Cmd(bool enable)
{
    if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFAGC_FMT, (unsigned)(enable ? 1u : 0u));
    } else {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSAGC_FMT, (unsigned)(enable ? 1u : 0u));
    }
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetAGC_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  7. Sidetone
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetSidetone_Cmd(uint8_t level)
{
    if (level > 15) level = 15;
    if (s_active_profile == MODEM_AUDIO_PROFILE_HANDSFREE) {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHFSD_FMT, (unsigned)level);
    } else {
        snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SHSSD_FMT, (unsigned)level);
    }
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetSidetone_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  8. Codec status query
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_QueryCodec_Cmd(void)
{
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(AT_CODECINFO);
}

bool Audio_QueryCodec_Poll(Modem_Result *result, Modem_CodecInfo *info)
{
    if (!poll_ok_error(result)) return false;

    if (*result == MODEM_OK && info != NULL) {
        /* Parse #CODECINFO response — best effort.
         * Response format varies by modem firmware version. */
        info->codec_type      = 1;   /* 1 = MAX9867 (assumed) */
        info->sample_rate_khz = 8;   /* Default voice = 8 kHz */
        info->bit_depth       = 16;
        info->active          = true;

        /* Try to extract rate from accumulator if present */
        const char *acc = Modem_AT_GetAccumulator();
        const char *p = strstr(acc, "#CODECINFO:");
        if (p != NULL) {
            /* Format is modem-firmware dependent; leave defaults */
            (void)p;
        }
    }
    return true;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  9. Audio loopback test
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetLoopback_Cmd(bool enable)
{
    /* AT#SRP=0 → normal, AT#SRP=2 → local analog loopback */
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_SRP_FMT, (unsigned)(enable ? 2u : 0u));
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetLoopback_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  10. Tone generation / ringer
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_SetRingerVolume_Cmd(uint8_t level)
{
    if (level > 100) level = 100;
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_CRSL_FMT, (unsigned)level);
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetRingerVolume_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

void Audio_SetToneVolume_Cmd(uint8_t tone_class, uint8_t level)
{
    if (level > 5) level = 5;
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_TSVOL_FMT,
             (unsigned)tone_class, (unsigned)level);
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_DEFAULT);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_SetToneVolume_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  11. USB audio
 * ══════════════════════════════════════════════════════════════════════════ */

void Audio_ConfigUSB_Cmd(uint8_t composition)
{
    snprintf(s_cmd_buf, sizeof(s_cmd_buf), AT_USBCFG_FMT, (unsigned)composition);
    Modem_AT_BeginExchange(AUDIO_TIMEOUT_USBCFG);
    Modem_AT_SendCmd(s_cmd_buf);
}

bool Audio_ConfigUSB_Poll(Modem_Result *result)
{
    return poll_ok_error(result);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  12. Default configuration accessor
 * ══════════════════════════════════════════════════════════════════════════ */

const Modem_AudioConfig *Audio_GetDefaultConfig(void)
{
    return &s_default_config;
}
