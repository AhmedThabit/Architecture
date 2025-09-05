#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "device.h"                             // for LATx/TRISx if you use LED
#include "esp32_comm.h"                         // ESP32_SendFrame(...)
#include "peripheral/uart/plib_uart1.h"         // UART1_SerialSetup, etc.
#include "peripheral/uart/plib_uart_common.h"   // UART_SERIAL_SETUP

// extern time base (from your Timer1 tick)
extern volatile uint32_t msTicks;

/* -------------------- Protocol definitions -------------------- */
enum {
    OPC_GET  = 0x01,
    OPC_SET  = 0x02,
    OPC_PING = 0x03,
    OPC_ECHO = 0x04,
};

enum {
    ST_OK         = 0x00,
    ST_BAD_LEN    = 0x01,
    ST_UNKNOWN_OP = 0x02,
    ST_BAD_TLV    = 0x03,
    ST_APPLY_FAIL = 0x04,
};

/* Existing tags */
enum {
    T_FW_VERSION   = 0x01, // string
    T_UPTIME_MS    = 0x02, // u32 LE
    T_LED_STATE    = 0x10, // u8  (0=off,1=on)
    T_UART1_BAUD   = 0x11, // u32 LE
};

/* New status/telemetry tags */
enum {
    T_INPUTS       = 0x30, // u8 bitmask
    T_OUTPUTS      = 0x31, // u8 bitmask
    T_MOIST_T10    = 0x32, // u16 LE (tenths of %)
    T_ALARM_CODE   = 0x33, // u8
    T_RSSI_DBM     = 0x35, // s8
    T_UPTIME_S     = 0x37, // u32 LE seconds
};

/* Subcommand for alarm control */
enum {
    SUB_ALARMCTL   = 0x40,
    T_ALARM_RESET  = 0x01,
};

/* ACK tag for "what we applied" */
enum { T_ACK = 0xA0 };

/* Weak HAL hooks to read actual hardware (override these in your project) */
__attribute__((weak)) uint8_t  HAL_get_inputs(void)           { return 4; }           // bits IN3..IN0
__attribute__((weak)) uint8_t  HAL_get_outputs(void)          { return 16; }           // bits OUT3..OUT0
__attribute__((weak)) uint16_t HAL_get_moisture_tenths(void)  { return 7.5; }           // 451 -> 45.1%
__attribute__((weak)) uint8_t  HAL_get_alarm_code(void)       { return 2; }           // 0 = none
__attribute__((weak)) int8_t   HAL_get_rssi_dbm(void)         { return -60; }         // if unknown, pick a sane default
__attribute__((weak)) void     HAL_reset_alarm(void)          { /* clear latch */ }
__attribute__((weak)) uint32_t HAL_get_uptime_ms(void)        { return msTicks; }
__attribute__((weak)) uint8_t  HAL_get_led_state(void)        { return (LATA & (1u<<15)) ? 1u : 0u; }
__attribute__((weak)) uint32_t HAL_get_uart1_baud(void)       { return 115200u; }      // override with real baud

/* -------------------- Hardware helpers -------------------- */
static inline void led_set(bool on) {
    ANSELACLR = (1u << 15);
    TRISACLR  = (1u << 15);
    if (on) LATASET = (1u << 15);
    else    LATACLR = (1u << 15);
}

static inline uint32_t PBCLK_Hz(void) {
    return UART1_FrequencyGet()/2;   // adjust if you change clocks
}

/* -------------------- TLV helpers -------------------- */
static inline bool put_u16le(uint8_t* b, size_t cap, size_t* idx, uint16_t v) {
    if (*idx + 2 > cap) return false;
    b[(*idx)++] = (uint8_t)(v & 0xFF);
    b[(*idx)++] = (uint8_t)((v >> 8) & 0xFF);
    return true;
}
static inline bool put_u32le(uint8_t* b, size_t cap, size_t* idx, uint32_t v) {
    if (*idx + 4 > cap) return false;
    b[(*idx)++] = (uint8_t)(v & 0xFF);
    b[(*idx)++] = (uint8_t)((v >> 8) & 0xFF);
    b[(*idx)++] = (uint8_t)((v >> 16) & 0xFF);
    b[(*idx)++] = (uint8_t)((v >> 24) & 0xFF);
    return true;
}
static inline bool put_tlv(uint8_t* b, size_t cap, size_t* idx, uint8_t t, const void* v, uint8_t l) {
    if (*idx + 2u + l > cap) return false;
    b[(*idx)++] = t;
    b[(*idx)++] = l;
    if (l) memcpy(&b[*idx], v, l);
    *idx += l;
    return true;
}
static inline bool put_tlv_u16(uint8_t* b, size_t cap, size_t* idx, uint8_t t, uint16_t v) {
    if (*idx + 2 + 2 > cap) return false;
    b[(*idx)++] = t; b[(*idx)++] = 2;
    return put_u16le(b, cap, idx, v);
}
static inline bool put_tlv_u32(uint8_t* b, size_t cap, size_t* idx, uint8_t t, uint32_t v) {
    if (*idx + 2 + 4 > cap) return false;
    b[(*idx)++] = t; b[(*idx)++] = 4;
    return put_u32le(b, cap, idx, v);
}

/* Return firmware string */
static bool tlv_fw_version(uint8_t* out, size_t cap, size_t* idx) {
    static const char fw[] = "PIC32MM-APP 1.0";
    return put_tlv(out, cap, idx, T_FW_VERSION, fw, (uint8_t)strlen(fw));
}

/* GET single TLV */
static bool tlv_get_reply(uint8_t tag, uint8_t* out, size_t cap, size_t* idx) {
    switch (tag) {
        case T_FW_VERSION: return tlv_fw_version(out, cap, idx);
        case T_UPTIME_MS:  return put_tlv_u32(out, cap, idx, T_UPTIME_MS, HAL_get_uptime_ms());
        case T_UART1_BAUD: return put_tlv_u32(out, cap, idx, T_UART1_BAUD, HAL_get_uart1_baud());
        case T_LED_STATE: {
            uint8_t v = HAL_get_led_state();
            return put_tlv(out, cap, idx, T_LED_STATE, &v, 1);
        }
        /* New aggregate tag: pack all 0x30..0x37 in one shot */
        case T_INPUTS: {
            uint8_t in   = HAL_get_inputs();
            uint8_t outm = HAL_get_outputs();
            uint16_t m10 = HAL_get_moisture_tenths();
            uint8_t  alm = HAL_get_alarm_code();
            int8_t   rssi= HAL_get_rssi_dbm();
            uint32_t ups = put_tlv_u32(out, cap, idx, T_UPTIME_MS, HAL_get_uptime_ms());
            if (!put_tlv(out, cap, idx, T_INPUTS,  &in,  1)) return false;
            if (!put_tlv(out, cap, idx, T_OUTPUTS, &outm, 1)) return false;
            if (!put_tlv_u16(out, cap, idx, T_MOIST_T10, m10)) return false;
            if (!put_tlv(out, cap, idx, T_ALARM_CODE, &alm, 1)) return false;
            if (!put_tlv(out, cap, idx, T_RSSI_DBM,  &rssi,1)) return false;
            if (!put_tlv_u32(out, cap, idx, T_UPTIME_S, ups))   return false;
            return true;
        }
        default:
            /* Unknown tag? ignore gracefully */
            return true;
    }
}

/* Apply TLVs for SET command */
static bool tlv_apply(const uint8_t* p, size_t plen, uint8_t* st_out) {
    if (plen < 2) { *st_out = ST_BAD_TLV; return false; }
    uint8_t tag = p[0], len = p[1];
    if (plen < (size_t)(2 + len)) { *st_out = ST_BAD_TLV; return false; }

    switch (tag) {
        case T_LED_STATE:
            if (len != 1) { *st_out = ST_BAD_TLV; return false; }
            led_set(p[2] ? true : false);
            *st_out = ST_OK;
            return true;
        case T_UART1_BAUD: {
            if (len != 4) { *st_out = ST_BAD_TLV; return false; }
            uint32_t baud = (uint32_t)p[2]
                          | ((uint32_t)p[3] << 8)
                          | ((uint32_t)p[4] << 16)
                          | ((uint32_t)p[5] << 24);
            if (baud < 1200 || baud > 921600) { *st_out = ST_APPLY_FAIL; return false; }
            UART_SERIAL_SETUP s = { .baudRate = baud, .dataWidth = UART_DATA_8_BIT,
                                    .parity = UART_PARITY_NONE, .stopBits = UART_STOP_1_BIT };
            bool ok = UART1_SerialSetup(&s, PBCLK_Hz()*2);
            *st_out = ok ? ST_OK : ST_APPLY_FAIL;
            return ok;
        }
        default:
            *st_out = ST_BAD_TLV;
            return false;
    }
}

/* Main handler for each received payload from ESP32 */
void Esp_HandleFrame(const uint8_t* payload, size_t len) {
    if (len == 0) return;
    uint8_t op = payload[0];
    const uint8_t* q = payload + 1;
    size_t rem = (len > 0) ? (len - 1) : 0;

    uint8_t rsp[320];
    size_t ri = 0;
    uint8_t status = ST_OK;
    uint8_t rop = op | 0x80; // reply opcode

    rsp[ri++] = rop;
    rsp[ri++] = ST_OK; // placeholder

    switch (op) {
        case OPC_PING:
            break; // no TLVs
        case OPC_ECHO:
            if (rem <= 255) {
                if (!put_tlv(rsp, sizeof(rsp), &ri, 0xEE, q, (uint8_t)rem)) status = ST_BAD_LEN;
            } else status = ST_BAD_LEN;
            break;
        case OPC_SET: {
            // Check for ALARMCTL sub-command
            if (rem >= 1 && q[0] == SUB_ALARMCTL) {
                q++; rem--;
                bool did = false;
                while (rem >= 2) {
                    uint8_t l = q[1];
                    if (rem < (size_t)(2 + l)) { status = ST_BAD_TLV; break; }
                    if (q[0] == T_ALARM_RESET && l == 1 && q[2] == 1) {
                        HAL_reset_alarm();
                        did = true;
                    }
                    rem -= (2 + l); q += (2 + l);
                }
                if (status == ST_OK && did) {
                    uint8_t one = 1;
                    (void)put_tlv(rsp, sizeof(rsp), &ri, T_ACK, &one, 1); // A0 01 01
                } else if (status == ST_OK && !did) {
                    status = ST_BAD_TLV;
                }
                break;
            }
            // Legacy SET: treat TLVs directly
            uint8_t last_led = 0xFF;
            while (rem >= 2) {
                uint8_t l = q[1];
                if (rem < (size_t)(2 + l)) { status = ST_BAD_TLV; break; }
                if (q[0] == T_LED_STATE && l == 1) last_led = q[2];
                uint8_t st_each = ST_OK;
                bool ok = tlv_apply(q, 2 + l, &st_each);
                if (!ok && status == ST_OK) status = st_each;
                q   += 2 + l;
                rem -= 2 + l;
            }
            if (status == ST_OK && last_led != 0xFF) {
                (void)put_tlv(rsp, sizeof rsp, &ri, T_ACK, &last_led, 1);
            }
            break;
        }
        case OPC_GET: {
            while (rem >= 2) {
                uint8_t tag = q[0], l = q[1];
                if (rem < (size_t)(2 + l)) { status = ST_BAD_TLV; break; }
                if (!tlv_get_reply(tag, rsp, sizeof(rsp), &ri)) {
                    status = ST_BAD_LEN; break;
                }
                q += 2 + l; rem -= 2 + l;
            }
            if (rem != 0 && status == ST_OK) status = ST_BAD_TLV;
            break;
        }
        default:
            status = ST_UNKNOWN_OP;
            break;
    }
    rsp[1] = status;
    ESP32_SendFrame(rsp, ri);
}
