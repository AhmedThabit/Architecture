#pragma once
#include <stdint.h>

// OP codes
#define OP_GET   0x01
#define OP_SET   0x02
#define OP_SAVE  0x03
#define OP_LOAD  0x04
#define OP_RESET 0x05

// SUB domains (keep your old ones; append new)
#define SUB_STATUS     0x30
#define SUB_PHONEBOOK  0x22
#define SUB_ALARMCTL   0x40
#define SUB_TESTSMS    0x41

// TLV tags for STATUS reply
#define TAG_IN_MASK    0x30   // u8 bit0..3
#define TAG_OUT_MASK   0x31   // u8 bit0..3
#define TAG_MOIST_PCT  0x32   // u16 (0..1000 = 0.0..100.0%)
#define TAG_ALARM_CODE 0x33   // u8 (0=None, 1=MoistHi, 2=In1.. etc.)
#define TAG_ALARM_TEXT 0x34   // str (optional)
#define TAG_RSSI       0x35   // s8  (dBm)
#define TAG_UPTIME_S   0x37   // u32 seconds

// Phonebook TLVs
#define TAG_PB_MODE    0x01   // u8 (reserve)
#define TAG_PB_SLOT    0x02   // u8 (1..16)
#define TAG_PB_NUMBER  0x03   // str (+20...)
#define TAG_PB_CLEAR   0x04   // u8 (1=clear all)

// ALARM control
#define TAG_ALARM_RST  0x01   // u8 (1=reset)
// TLV tags
#define T_SMS_EN   0x20    // NEW: SMS enable flag (u8: 0=off, 1=on)


// framing helpers you already use (length, CRC16-CCITT) stay unchanged
