/**
 * @file    handlers.c
 * @brief   Application-layer request handlers for ESP32 GET/SET commands.
 *
 * @details These functions are called by the ESP32 protocol handler to
 *          build TLV responses (GET) or apply settings (SET).
 *          They use service-layer and HAL APIs — never touch hardware directly.
 */

#include "common/proto.h"
#include "hal/io_monitor/io_monitor.h"
#include "common/schema.h"
#include "services/gsm/gsm_service.h"

#include <string.h>

/* ── External references ────────────────────────────────────────────────── */

extern uint8_t AlarmMgr_GetCode(void);
extern void    AlarmMgr_Reset(void);

/* ── Tag definitions (must match esp32_proto tags) ──────────────────────── */

#define TAG_IN_MASK     0x30
#define TAG_OUT_MASK    0x31
#define TAG_MOIST_PCT   0x32
#define TAG_ALARM_CODE  0x33
#define TAG_RSSI        0x35
#define TAG_PB_CLEAR    0x44
#define TAG_PB_SLOT     0x45
#define TAG_PB_NUMBER   0x46

/* ── Handlers ───────────────────────────────────────────────────────────── */

int Handle_GetStatus(uint8_t *out, int max)
{
    uint8_t *p = out;
    uint8_t  in_mask  = IO_GetInputsMask();
    uint8_t  out_mask = IO_GetOutputsMask();
    uint16_t moist    = IO_GetMoistPct10();
    uint8_t  alarm    = AlarmMgr_GetCode();
    int8_t   rssi     = -70;  /* TODO: use GSM_Cmd_QuerySignal / poll */

    *p++ = TAG_IN_MASK;    *p++ = 1; *p++ = in_mask;
    *p++ = TAG_OUT_MASK;   *p++ = 1; *p++ = out_mask;
    *p++ = TAG_MOIST_PCT;  *p++ = 2; *p++ = (uint8_t)(moist & 0xFF);
                                      *p++ = (uint8_t)((moist >> 8) & 0xFF);
    *p++ = TAG_ALARM_CODE; *p++ = 1; *p++ = alarm;
    *p++ = TAG_RSSI;       *p++ = 1; *p++ = (uint8_t)rssi;

    return (int)(p - out);
}

int Handle_ResetAlarm(uint8_t *out, int max)
{
    (void)out;
    (void)max;
    AlarmMgr_Reset();
    return 0;
}

int Handle_SetPhonebook(const uint8_t *in, int ilen)
{
    int i = 0;

    while (i < ilen) {
        uint8_t tag = in[i++];
        uint8_t len = in[i++];
        const uint8_t *v = &in[i];
        i += len;

        if (tag == TAG_PB_CLEAR && len == 1 && v[0] == 1) {
            memset(&g_device_cfg.phonebook, 0, sizeof(g_device_cfg.phonebook));
        }
        else if (tag == TAG_PB_SLOT) {
            uint8_t slot = v[0];
            /* Expect next TLV to be TAG_PB_NUMBER */
            if (i < ilen && in[i] == TAG_PB_NUMBER) {
                uint8_t nlen = in[i + 1];
                const char *num_str = (const char *)&in[i + 2];
                if (slot >= 1 && slot <= CFG_PHONEBOOK_MAX_ENTRIES && nlen < CFG_PHONEBOOK_NUM_LEN) {
                    memcpy(g_device_cfg.phonebook.numbers[slot - 1], num_str, nlen);
                    g_device_cfg.phonebook.numbers[slot - 1][nlen] = '\0';
                }
                i += 2 + nlen;
            }
        }
    }

    Cfg_Save();
    return 0;
}
