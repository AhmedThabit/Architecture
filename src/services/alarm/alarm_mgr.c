/**
 * @file    alarm_mgr.c
 * @brief   Alarm manager — monitors inputs, manages alarm state & notifications.
 *
 * @details Implements per-input alarm detection with configurable debounce,
 *          trigger delay, clear delay, latch, and output mapping.
 *          Also monitors moisture, battery voltage, and mains power.
 *
 *          Per-input configuration comes from g_device_cfg.inputs[i]
 *          (InputChannelCfg struct in schema.h), which the mobile app
 *          can read/write via the ESP32 TLV protocol.
 *
 *          The notification sequencing (dial, play, SMS) is driven by the
 *          AlarmThread protothread which calls the state machine accessors.
 *          This file only manages alarm STATE, not the AT command flow.
 */

#include "alarm_mgr.h"
#include "hal/io_monitor/io_monitor.h"
#include "common/schema.h"
#include "config/app_config.h"
#include "hal/bsp/bsp.h"

#include <string.h>
#include <stdio.h>

/* ══════════════════════════════════════════════════════════════════════════
 *  Per-input alarm state
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    INPUT_ALARM_IDLE = 0,
    INPUT_ALARM_PENDING,
    INPUT_ALARM_TRIGGERED,
    INPUT_ALARM_NOTIFYING,
    INPUT_ALARM_NOTIFIED,
    INPUT_ALARM_CLEARING,
} InputAlarmState;

typedef struct {
    InputAlarmState  state;
    uint32_t         timer;
} InputAlarm;

/* ══════════════════════════════════════════════════════════════════════════
 *  Module state
 * ══════════════════════════════════════════════════════════════════════════ */

static InputAlarm   s_input_alarms[CFG_DIN_COUNT];
static bool         s_moisture_alarm;
static bool         s_battery_low;
static bool         s_mains_fail;
static bool         s_prev_mains;

static uint8_t      s_active_flags;
static bool         s_need_notify;
static uint8_t      s_notify_flags;

static NotifyState  s_notify_state = NOTIFY_IDLE;
static int          s_dial_index   = 0;
static uint8_t      s_dial_attempts = 0;

/* ══════════════════════════════════════════════════════════════════════════
 *  Initialisation
 * ══════════════════════════════════════════════════════════════════════════ */

void AlarmMgr_Init(void)
{
    memset(s_input_alarms, 0, sizeof(s_input_alarms));
    s_moisture_alarm = false;
    s_battery_low    = false;
    s_mains_fail     = false;
    s_prev_mains     = true;
    s_active_flags   = 0;
    s_need_notify    = false;
    s_notify_flags   = 0;
    s_notify_state   = NOTIFY_IDLE;
    s_dial_index     = 0;
    s_dial_attempts  = 0;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Periodic scan
 * ══════════════════════════════════════════════════════════════════════════ */

void AlarmMgr_Task(void)
{
    uint32_t now = g_ms_ticks;
    uint8_t  inputs = IO_GetDebouncedInputs();
    uint8_t  new_flags = 0;

    /* ── Digital input alarm detection (per-input config) ───────────── */
    for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
        const InputChannelCfg *icfg = &g_device_cfg.inputs[i];
        InputAlarm *ia = &s_input_alarms[i];
        bool active = (inputs >> i) & 0x01;

        /* Skip disabled inputs */
        if (!icfg->enabled) {
            if (ia->state != INPUT_ALARM_IDLE) {
                ia->state = INPUT_ALARM_IDLE;
                if (icfg->output_map < CFG_DOUT_COUNT)
                    IO_ClearOutput(icfg->output_map);
            }
            continue;
        }

        /* Per-input delays */
        uint16_t trig_delay = icfg->trigger_delay_ms;
        uint16_t clr_delay  = icfg->clear_delay_ms;

        switch (ia->state) {

        case INPUT_ALARM_IDLE:
            if (active) {
                ia->state = INPUT_ALARM_PENDING;
                ia->timer = now;
            }
            break;

        case INPUT_ALARM_PENDING:
            if (!active) {
                ia->state = INPUT_ALARM_IDLE;
            } else if ((now - ia->timer) >= trig_delay) {
                ia->state = INPUT_ALARM_TRIGGERED;
                new_flags |= (1u << i);
            }
            break;

        case INPUT_ALARM_TRIGGERED:
            new_flags |= (1u << i);
            break;

        case INPUT_ALARM_NOTIFYING:
            new_flags |= (1u << i);
            break;

        case INPUT_ALARM_NOTIFIED:
            new_flags |= (1u << i);
            if (!active && !icfg->latch) {
                ia->state = INPUT_ALARM_CLEARING;
                ia->timer = now;
            }
            break;

        case INPUT_ALARM_CLEARING:
            if (active) {
                ia->state = INPUT_ALARM_NOTIFIED;
                new_flags |= (1u << i);
            } else if ((now - ia->timer) >= clr_delay) {
                ia->state = INPUT_ALARM_IDLE;
            } else {
                new_flags |= (1u << i);
            }
            break;
        }

        /* Activate mapped output relay */
        if (ia->state >= INPUT_ALARM_TRIGGERED && ia->state <= INPUT_ALARM_NOTIFIED) {
            if (icfg->output_map < CFG_DOUT_COUNT) {
                IO_SetOutput(icfg->output_map);
            }
        } else if (ia->state == INPUT_ALARM_IDLE) {
            if (icfg->output_map < CFG_DOUT_COUNT) {
                IO_ClearOutput(icfg->output_map);
            }
        }
    }

    /* ── Moisture alarm ─────────────────────────────────────────────── */
    if (IO_GetMoistPct10() >= g_device_cfg.io.moist_thr &&
        g_device_cfg.io.moist_thr > 0) {
        if (!s_moisture_alarm) {
            s_moisture_alarm = true;
            new_flags |= ALARM_MOISTURE;
        }
    } else {
        s_moisture_alarm = false;
    }
    if (s_moisture_alarm) new_flags |= ALARM_MOISTURE;

    /* ── Battery monitoring ─────────────────────────────────────────── */
    uint16_t batt_mv = IO_GetBatteryMV();
    uint16_t batt_thr = g_device_cfg.alarm.battery_low_mv;
    if (batt_thr == 0) batt_thr = CFG_BATTERY_LOW_MV;

    if (batt_mv < batt_thr && batt_mv > 0) {
        if (!s_battery_low) {
            s_battery_low = true;
            new_flags |= ALARM_BATTERY_LOW;
        }
    } else {
        s_battery_low = false;
    }
    if (s_battery_low) new_flags |= ALARM_BATTERY_LOW;

    /* ── Mains power monitoring ─────────────────────────────────────── */
    bool mains_now = IO_IsMainsPowerPresent();
    if (s_prev_mains && !mains_now) {
        s_mains_fail = true;
        new_flags |= ALARM_MAINS_FAIL;
    }
    if (mains_now) {
        s_mains_fail = false;
    }
    s_prev_mains = mains_now;
    if (s_mains_fail) new_flags |= ALARM_MAINS_FAIL;

    /* ── Check for new alarms that need notification ────────────────── */
    uint8_t newly_triggered = new_flags & ~s_active_flags;
    if (newly_triggered && s_notify_state == NOTIFY_IDLE) {
        s_need_notify  = true;
        s_notify_flags = new_flags;
        s_dial_index   = 0;
        s_dial_attempts = 0;

        for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
            if (s_input_alarms[i].state == INPUT_ALARM_TRIGGERED) {
                s_input_alarms[i].state = INPUT_ALARM_NOTIFYING;
            }
        }
    }

    s_active_flags = new_flags;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Query functions
 * ══════════════════════════════════════════════════════════════════════════ */

uint8_t AlarmMgr_GetActiveAlarms(void)
{
    return s_active_flags;
}

uint8_t AlarmMgr_GetCode(void)
{
    if (s_active_flags & ALARM_MOISTURE)    return 1;
    if (s_active_flags & ALARM_INPUT_1)     return 2;
    if (s_active_flags & ALARM_INPUT_2)     return 3;
    if (s_active_flags & ALARM_INPUT_3)     return 4;
    if (s_active_flags & ALARM_INPUT_4)     return 5;
    if (s_active_flags & ALARM_BATTERY_LOW) return 6;
    if (s_active_flags & ALARM_MAINS_FAIL)  return 7;
    return 0;
}

void AlarmMgr_Reset(void)
{
    for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
        s_input_alarms[i].state = INPUT_ALARM_IDLE;
        if (g_device_cfg.inputs[i].output_map < CFG_DOUT_COUNT) {
            IO_ClearOutput(g_device_cfg.inputs[i].output_map);
        }
    }
    s_moisture_alarm = false;
    s_battery_low    = false;
    s_mains_fail     = false;
    s_active_flags   = 0;
    s_need_notify    = false;
    s_notify_state   = NOTIFY_IDLE;
    s_dial_index     = 0;
    s_dial_attempts  = 0;
}

void AlarmMgr_Acknowledge(AlarmFlags flag)
{
    for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
        if ((flag & (1u << i)) && s_input_alarms[i].state == INPUT_ALARM_NOTIFIED) {
            s_input_alarms[i].state = INPUT_ALARM_IDLE;
            if (g_device_cfg.inputs[i].output_map < CFG_DOUT_COUNT) {
                IO_ClearOutput(g_device_cfg.inputs[i].output_map);
            }
        }
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Notification state machine accessors
 * ══════════════════════════════════════════════════════════════════════════ */

bool AlarmMgr_NeedNotify(void)     { return s_need_notify; }
NotifyState AlarmMgr_GetNotifyState(void)  { return s_notify_state; }
void AlarmMgr_SetNotifyState(NotifyState s) { s_notify_state = s; }

int AlarmMgr_GetNextDialIndex(void)
{
    for (int i = s_dial_index; i < CFG_PHONEBOOK_MAX_ENTRIES; i++) {
        if (g_device_cfg.phonebook.numbers[i][0] != '\0') {
            return i;
        }
    }
    return -1;
}

void AlarmMgr_AdvanceDialIndex(void)
{
    s_dial_index++;
    s_dial_attempts++;
    if (s_dial_index >= CFG_PHONEBOOK_MAX_ENTRIES) {
        s_dial_index = 0;
    }
}

void AlarmMgr_GetAlarmSmsText(char *out, size_t outLen)
{
    if (!out || outLen == 0) return;

    uint8_t inputs = IO_GetDebouncedInputs();
    uint16_t batt  = IO_GetBatteryMV();
    bool mains     = IO_IsMainsPowerPresent();

    /* Build message with input labels for triggered inputs */
    int pos = snprintf(out, outLen, "EDAC8000 ALARM:");

    for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
        if (inputs & (1u << i)) {
            const char *label = g_device_cfg.inputs[i].label;
            if (label[0] != '\0') {
                pos += snprintf(out + pos, outLen - pos, " %s", label);
            } else {
                pos += snprintf(out + pos, outLen - pos, " IN%d", i + 1);
            }
        }
        if ((size_t)pos >= outLen - 1) break;
    }

    if (s_moisture_alarm) pos += snprintf(out + pos, outLen - pos, " MOISTURE");
    if (s_battery_low)    pos += snprintf(out + pos, outLen - pos, " BATT_LOW");
    if (s_mains_fail)     pos += snprintf(out + pos, outLen - pos, " MAINS_FAIL");

    snprintf(out + pos, outLen - pos, " V=%u.%uV M=%s",
        batt / 1000, (batt % 1000) / 100,
        mains ? "OK" : "FAIL");
}

void AlarmMgr_NotifyComplete(void)
{
    s_need_notify  = false;
    s_notify_state = NOTIFY_IDLE;
    s_dial_index   = 0;
    s_dial_attempts = 0;

    for (uint8_t i = 0; i < CFG_DIN_COUNT; i++) {
        if (s_input_alarms[i].state == INPUT_ALARM_NOTIFYING) {
            s_input_alarms[i].state = INPUT_ALARM_NOTIFIED;
        }
    }
}
