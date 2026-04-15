/**
 * @file    alarm_mgr.h
 * @brief   Alarm manager -- monitors inputs, sequences notifications.
 *
 * @details The alarm manager is the core product logic for the EDAC 8000.
 *          It monitors digital inputs (debounced), detects alarm conditions,
 *          and drives the notification sequence:
 *
 *            1. Activate mapped output relay (siren/light)
 *            2. Dial phone numbers in sequence from the phonebook
 *            3. Play a voice message from the SD card during the call
 *            4. If no answer -> dial next number, retry up to max attempts
 *            5. Send SMS to all programmed numbers with alarm text
 *            6. Send battery/mains alerts independently
 *
 *          Per-input configuration (stored in schema.h / flash):
 *            - NO/NC polarity (in_no_nc bitmask)
 *            - Latch mode (in_latch bitmask)
 *            - Trigger delay (CFG_ALARM_TRIGGER_DELAY_MS)
 *
 *          The AlarmThread protothread in protothreads.c calls
 *          AlarmMgr_Task() periodically and then drives the notification
 *          state machine using the modem API.
 */

#ifndef ALARM_MGR_H
#define ALARM_MGR_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -- Alarm event types --------------------------------------------------- */

typedef enum {
    ALARM_NONE           = 0,
    ALARM_INPUT_1        = (1u << 0),
    ALARM_INPUT_2        = (1u << 1),
    ALARM_INPUT_3        = (1u << 2),
    ALARM_INPUT_4        = (1u << 3),
    ALARM_MOISTURE       = (1u << 4),
    ALARM_BATTERY_LOW    = (1u << 5),
    ALARM_MAINS_FAIL     = (1u << 6),
} AlarmFlags;

/** Notification state machine states. */
typedef enum {
    NOTIFY_IDLE = 0,        /**< No active alarm notification              */
    NOTIFY_START_CALL,      /**< About to dial the next number             */
    NOTIFY_WAIT_DIAL,       /**< Waiting for ATD to return OK              */
    NOTIFY_WAIT_RING,       /**< Ringing, waiting for answer or timeout    */
    NOTIFY_IN_CALL,         /**< Call connected, playing voice message     */
    NOTIFY_HANGUP,          /**< Hanging up after message played           */
    NOTIFY_WAIT_HANGUP,     /**< Waiting for ATH to complete               */
    NOTIFY_SEND_SMS,        /**< Sending SMS to current number             */
    NOTIFY_WAIT_SMS,        /**< Waiting for SMS send to complete          */
    NOTIFY_INTER_CALL_WAIT, /**< Delay between call attempts               */
    NOTIFY_DONE,            /**< All numbers tried, notification complete  */
} NotifyState;

/* -- Public API ---------------------------------------------------------- */

/** Initialise the alarm manager.  Call once at startup. */
void AlarmMgr_Init(void);

/** Periodic alarm scan -- checks debounced inputs, moisture,
 *  battery, and mains.  Call every CFG_ALARM_SCAN_MS from AlarmThread. */
void AlarmMgr_Task(void);

/** Get the current active alarm flags (bitmask of AlarmFlags). */
uint8_t AlarmMgr_GetActiveAlarms(void);

/** Legacy compatibility -- returns first active alarm code.
 *  0=none, 1=moisture, 2..5=input 1..4, 6=battery, 7=mains */
uint8_t AlarmMgr_GetCode(void);

/** Acknowledge/reset all alarms.  Clears latched alarms. */
void AlarmMgr_Reset(void);

/** Acknowledge a specific alarm by flag. */
void AlarmMgr_Acknowledge(AlarmFlags flag);

/** Check if the notification state machine needs to run. */
bool AlarmMgr_NeedNotify(void);

/** Get the current notification state (for the AlarmThread). */
NotifyState AlarmMgr_GetNotifyState(void);

/** Set the notification state (driven by AlarmThread). */
void AlarmMgr_SetNotifyState(NotifyState state);

/** Get the phone number index to dial next (0..15).  Returns -1 if done. */
int AlarmMgr_GetNextDialIndex(void);

/** Advance to the next phone number in the call list. */
void AlarmMgr_AdvanceDialIndex(void);

/** Get the alarm SMS text for the current alarm event. */
void AlarmMgr_GetAlarmSmsText(char *out, size_t outLen);

/** Mark notification as complete for the current alarm event. */
void AlarmMgr_NotifyComplete(void);

#endif /* ALARM_MGR_H */
