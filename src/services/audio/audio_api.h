/**
 * @file    audio_api.h
 * @brief   Audio codec abstraction layer — controls MAX9867 via modem AT cmds.
 *
 * @details Hardware architecture:
 *
 *     ┌──────────┐   I2C (registers)   ┌──────────┐
 *     │ LE910C4  │────────────────────→│ MAX9867  │──→ Speaker / Headphone
 *     │  Modem   │   PCM (audio data)  │  Codec   │←── Microphone
 *     │ (Linux)  │←──────────────────→│          │
 *     └────┬─────┘                     └──────────┘
 *          │ UART3 (AT commands)
 *     ┌────┴─────┐
 *     │ PIC32MM  │  ← You are here. Controls audio via AT commands ONLY.
 *     └──────────┘
 *
 *     The modem's internal Linux/ALSA subsystem:
 *       - Configures MAX9867 I2C registers (clocking, gain, filters)
 *       - Streams PCM audio data during voice calls
 *       - Applies echo cancellation, noise reduction, AGC
 *
 *     The PIC32MM firmware:
 *       - Sends AT commands to configure audio path/volume/profile
 *       - Does NOT touch I2C or PCM — the modem handles everything
 *
 *  Clock source configuration:
 *     CFG_AUDIO_CLK_SOURCE in app_config.h selects the MCLK source:
 *       - MODEM_CODEC_CLK_MODEM   → Modem PCM_CLK drives MAX9867 MCLK
 *       - MODEM_CODEC_CLK_CRYSTAL → External oscillator drives MAX9867 MCLK
 *     The modem's ALSA driver auto-detects and configures NI/PSCLK accordingly.
 *
 *  Cmd/Poll pattern (same as all other modem_api operations):
 *
 *     Audio_Xxx_Cmd(params);
 *     PT_WAIT_UNTIL(pt, Audio_Xxx_Poll(&result));
 */

#ifndef AUDIO_API_H
#define AUDIO_API_H

#include <stdint.h>
#include <stdbool.h>
#include "services/modem_api/modem_api_types.h"

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  1. AUDIO INITIALISATION & FULL SETUP                                ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Full audio system initialisation (multi-step).
 *
 *         Internally executes this AT sequence:
 *           1. AT#PSEL=<profile>         Select audio profile (handset/HF)
 *           2. AT+CLVL=<level>           Set speaker volume
 *           3. AT#HFMICG=<gain>          Set microphone gain (handsfree)
 *              or AT#HSMICG=<gain>       Set microphone gain (handset)
 *           4. AT#SHFEC=<0|1>            Echo canceller on/off
 *           5. AT#SHFNR=<0|1>            Noise reduction on/off
 *           6. AT#SHFAGC=<0|1>           Auto gain control on/off
 *           7. AT#CODECINFO              Verify codec is active
 *
 * @param  config  Pointer to full audio configuration struct.
 *                 Pass NULL to use default config from app_config.h.
 */
void Audio_Init_Cmd(const Modem_AudioConfig *config);
bool Audio_Init_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  2. AUDIO PROFILE SELECTION                                          ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Select audio profile (handset or handsfree).
 *         AT#PSEL=<profile>
 *
 * @param  profile  MODEM_AUDIO_PROFILE_HANDSET or _HANDSFREE
 */
void Audio_SelectProfile_Cmd(Modem_AudioProfile profile);
bool Audio_SelectProfile_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  3. SPEAKER VOLUME CONTROL                                           ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Set speaker/earpiece volume.  AT+CLVL=<level>
 * @param  level  Volume 0–100 (mapped internally to modem range 0–5)
 */
void Audio_SetVolume_Cmd(uint8_t level);
bool Audio_SetVolume_Poll(Modem_Result *result);

/**
 * @brief  Query current speaker volume.  AT+CLVL?
 */
void Audio_GetVolume_Cmd(void);
bool Audio_GetVolume_Poll(Modem_Result *result, uint8_t *level);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  4. MICROPHONE GAIN CONTROL                                          ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Set microphone gain for the currently active profile.
 *         Handsfree:  AT#HFMICG=<gain>  (0–7)
 *         Handset:    AT#HSMICG=<gain>  (0–7)
 * @param  gain  0–100 (mapped internally to modem range 0–7)
 */
void Audio_SetMicGain_Cmd(uint8_t gain);
bool Audio_SetMicGain_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  5. MUTE CONTROL                                                     ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Mute/unmute microphone.  AT+CMUT=<0|1> */
void Audio_SetMute_Cmd(bool mute);
bool Audio_SetMute_Poll(Modem_Result *result);

/** Mute/unmute speaker (Telit-specific).  AT#SPKMUT=<0|1> */
void Audio_SetSpeakerMute_Cmd(bool mute);
bool Audio_SetSpeakerMute_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  6. ECHO CANCELLATION & NOISE REDUCTION                              ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Enable/disable echo canceller.  AT#SHFEC or AT#SHSEC */
void Audio_SetEchoCanceller_Cmd(bool enable);
bool Audio_SetEchoCanceller_Poll(Modem_Result *result);

/** Enable/disable noise reduction.  AT#SHFNR or AT#SHSNR */
void Audio_SetNoiseReduction_Cmd(bool enable);
bool Audio_SetNoiseReduction_Poll(Modem_Result *result);

/** Enable/disable automatic gain control.  AT#SHFAGC or AT#SHSAGC */
void Audio_SetAGC_Cmd(bool enable);
bool Audio_SetAGC_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  7. SIDETONE CONTROL                                                 ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Set sidetone level (local mic → local speaker feedback).
 * @param  level  0 = off, 1–15 = increasing feedback volume
 */
void Audio_SetSidetone_Cmd(uint8_t level);
bool Audio_SetSidetone_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  8. CODEC STATUS QUERY                                               ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Query codec status from the modem.  AT#CODECINFO */
void Audio_QueryCodec_Cmd(void);
bool Audio_QueryCodec_Poll(Modem_Result *result, Modem_CodecInfo *info);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  9. AUDIO LOOPBACK TEST                                              ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/**
 * @brief  Enable/disable audio loopback (mic → speaker, for testing).
 *         AT#SRP=<0|2>   (0=normal, 2=local loopback)
 */
void Audio_SetLoopback_Cmd(bool enable);
bool Audio_SetLoopback_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  10. TONE / RINGER VOLUME                                            ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Set alert/ringer volume.  AT+CRSL=<level> */
void Audio_SetRingerVolume_Cmd(uint8_t level);
bool Audio_SetRingerVolume_Poll(Modem_Result *result);

/**
 * @brief  Set tone class volumes.  AT#TSVOL=<class>,<level>
 * @param  tone_class  0=ringer, 1=SMS, 2=keypad, 3=supervisory
 * @param  level       Volume 0–5
 */
void Audio_SetToneVolume_Cmd(uint8_t tone_class, uint8_t level);
bool Audio_SetToneVolume_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  11. USB AUDIO (alternative to PCM codec)                            ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Configure USB audio composition.  AT#USBCFG=<id> (11=audio enabled) */
void Audio_ConfigUSB_Cmd(uint8_t composition);
bool Audio_ConfigUSB_Poll(Modem_Result *result);

/* ╔════════════════════════════════════════════════════════════════════════╗
 * ║  12. CONVENIENCE — DEFAULT CONFIG                                    ║
 * ╚════════════════════════════════════════════════════════════════════════╝ */

/** Get default audio configuration based on app_config.h defines. */
const Modem_AudioConfig *Audio_GetDefaultConfig(void);

#endif /* AUDIO_API_H */
