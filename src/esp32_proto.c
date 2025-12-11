#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "device.h"                             // for LATx/TRISx if you use LED
#include "esp32_comm.h"                         // ESP32_SendFrame(...)
#include "peripheral/uart/plib_uart1.h"         // UART1_SerialSetup, etc.
#include "peripheral/uart/plib_uart_common.h"   // UART_SERIAL_SETUP
#include "schema.h"        // <-- Add this line
#include "store.h"         // or wherever phonebook_* is declared
// extern time base (from your Timer1 tick)
extern volatile uint32_t msTicks;
extern DeviceCfg g_cfg; // if you need it for other things

// declare UART3_WriteString33 prototype or include the header
void UART3_WriteString33(const char *str); // forward declaration

static void uart3_write_hex(uint8_t byte) {
    const char hex[] = "0123456789ABCDEF";
    char out[3] = {hex[byte >> 4], hex[byte & 0x0F], 0};
    UART3_WriteString33(out);
}

/* -------------------- Protocol definitions -------------------- */
enum {
    OPC_GET = 0x01,
    OPC_SET = 0x02,
    OPC_PING = 0x03,
    OPC_ECHO = 0x04,
};

enum {
    ST_OK = 0x00,
    ST_BAD_LEN = 0x01,
    ST_UNKNOWN_OP = 0x02,
    ST_BAD_TLV = 0x03,
    ST_APPLY_FAIL = 0x04,
};

/* Existing tags */
enum {
    T_FW_VERSION = 0x01, // string
    T_UPTIME_MS = 0x02, // u32 LE
    T_LED_STATE = 0x10, // u8  (0=off,1=on)
    T_UART1_BAUD = 0x11, // u32 LE
};

/* New status/telemetry tags */
enum {
    T_INPUTS = 0x30, // u8 bitmask
    T_OUTPUTS = 0x31, // u8 bitmask
    T_MOIST_T10 = 0x32, // u16 LE (tenths of %)
    T_ALARM_CODE = 0x33, // u8
    T_RSSI_DBM = 0x35, // s8
    T_UPTIME_S = 0x37, // u32 LE seconds
};

/* Subcommand for alarm control */
enum {
    SUB_ALARMCTL = 0x40,
    T_ALARM_RESET = 0x01,
};

/* ACK tag for "what we applied" */
enum {
    T_ACK = 0xA0
};

/* Weak HAL hooks to read actual hardware (override these in your project) */
__attribute__((weak)) uint8_t HAL_get_inputs(void) {
    return 4;
} // bits IN3..IN0

__attribute__((weak)) uint8_t HAL_get_outputs(void) {
    return 16;
} // bits OUT3..OUT0

__attribute__((weak)) uint16_t HAL_get_moisture_tenths(void) {
    return 451;
} // 451 -> 45.1%

__attribute__((weak)) uint8_t HAL_get_alarm_code(void) {
    return 2;
} // 0 = none

__attribute__((weak)) int8_t HAL_get_rssi_dbm(void) {
    return -60;
} // if unknown, pick a sane default

__attribute__((weak)) void HAL_reset_alarm(void) {
    /* clear latch */
}

__attribute__((weak)) uint32_t HAL_get_uptime_ms(void) {
    return msTicks;
}

__attribute__((weak)) uint8_t HAL_get_led_state(void) {
    return (LATA & (1u << 15)) ? 1u : 0u;
}

__attribute__((weak)) uint32_t HAL_get_uart1_baud(void) {
    return 115200u;
} // override with real baud

/* -------------------- Hardware helpers -------------------- */
static inline void led_set(bool on) {
    ANSELACLR = (1u << 15);
    TRISACLR = (1u << 15);
    if (on) LATASET = (1u << 15);
    else LATACLR = (1u << 15);
}

static inline uint32_t PBCLK_Hz(void) {
    return UART1_FrequencyGet() / 2; // adjust if you change clocks
}

/* -------------------- TLV helpers -------------------- */
// at top of file

enum {
    T_SMS_EN = 0x20,
    T_PHONEBOOK_SET = 0x40, // slot + number (variable length)
    T_PHONEBOOK_LIST = 0x41, // no payload
    T_PHONEBOOK_DEF = 0x42, // one-byte slot index
};
extern void handle_sms_enable_cmd(uint8_t flag); // implemented in protothreads.c
extern uint8_t sms_get_enabled(void);

void send_phonebook_list(void) {
    uint8_t rsp[320];
    size_t idx = 0;

    UART3_WriteString33("send_phonebook_list() called\r\n");

    // Reply opcode: reply to OPC_SET (0x02) -> 0x82
    rsp[idx++] = OPC_SET | 0x80;
    rsp[idx++] = ST_OK; // status placeholder

    for (uint8_t slot = 0; slot < 16; slot++) {
        const char *num = phonebook_get_number(slot); // [flash] version
        if (num) {
            size_t len = strlen(num);

            UART3_WriteString33(" adding TLV for slot=");
            uart3_write_hex(slot);
            UART3_WriteString33(" num=\"");
            for (size_t i = 0; i < len; i++) {

                UART3_WriteString33((char[]) {
                    num[i], 0
                });
            }
            UART3_WriteString33("\"\r\n");

            // Check space: tag(1) + len(1) + slot(1) + digits(len)
            if (idx + 2 + 1 + len > sizeof (rsp)) {
                UART3_WriteString33(" buffer full, breaking\r\n");
                break;
            }

            rsp[idx++] = T_PHONEBOOK_SET; // 0x40
            rsp[idx++] = (uint8_t) (1 + len); // length = slot + digits
            rsp[idx++] = slot; // slot index
            memcpy(&rsp[idx], num, len);
            idx += len;
        }
    }

    uint8_t def = phonebook_get_default();
    if (def < 16 && idx + 3 <= sizeof (rsp)) {
        UART3_WriteString33(" adding default TLV, def=");
        uart3_write_hex(def);
        UART3_WriteString33("\r\n");

        rsp[idx++] = T_PHONEBOOK_DEF; // 0x42
        rsp[idx++] = 1; // length = 1 byte
        rsp[idx++] = def; // default index
    } else {
        UART3_WriteString33(" default TLV skipped (def>=16 or no space)\r\n");
    }

    UART3_WriteString33("send_phonebook_list: final frame bytes = ");
    for (size_t i = 0; i < idx; i++) {
        uart3_write_hex(rsp[i]);
        UART3_WriteString33(" ");
    }
    UART3_WriteString33("\r\n");

    ESP32_SendFrame(rsp, idx);
}

static inline bool put_u16le(uint8_t* b, size_t cap, size_t* idx, uint16_t v) {
    if (*idx + 2 > cap) return false;
    b[(*idx)++] = (uint8_t) (v & 0xFF);
    b[(*idx)++] = (uint8_t) ((v >> 8) & 0xFF);
    return true;
}

static inline bool put_u32le(uint8_t* b, size_t cap, size_t* idx, uint32_t v) {
    if (*idx + 4 > cap) return false;
    b[(*idx)++] = (uint8_t) (v & 0xFF);
    b[(*idx)++] = (uint8_t) ((v >> 8) & 0xFF);
    b[(*idx)++] = (uint8_t) ((v >> 16) & 0xFF);
    b[(*idx)++] = (uint8_t) ((v >> 24) & 0xFF);
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
    b[(*idx)++] = t;
    b[(*idx)++] = 2;
    return put_u16le(b, cap, idx, v);
}

static inline bool put_tlv_u32(uint8_t* b, size_t cap, size_t* idx, uint8_t t, uint32_t v) {
    if (*idx + 2 + 4 > cap) return false;
    b[(*idx)++] = t;
    b[(*idx)++] = 4;
    return put_u32le(b, cap, idx, v);
}

/* Return firmware string */
static bool tlv_fw_version(uint8_t* out, size_t cap, size_t* idx) {
    static const char fw[] = "PIC32MM-APP 1.0";
    return put_tlv(out, cap, idx, T_FW_VERSION, fw, (uint8_t) strlen(fw));
}

/* GET single TLV */
static bool tlv_get_reply(uint8_t tag, uint8_t* out, size_t cap, size_t* idx) {
    switch (tag) {
        case T_FW_VERSION: return tlv_fw_version(out, cap, idx);
        case T_UPTIME_MS: return put_tlv_u32(out, cap, idx, T_UPTIME_MS, HAL_get_uptime_ms());
        case T_UART1_BAUD: return put_tlv_u32(out, cap, idx, T_UART1_BAUD, HAL_get_uart1_baud());
        case T_LED_STATE:
        {
            uint8_t v = HAL_get_led_state();
            return put_tlv(out, cap, idx, T_LED_STATE, &v, 1);
        }
        case T_SMS_EN:
        {
            uint8_t v = sms_get_enabled();
            return put_tlv(out, cap, idx, T_SMS_EN, &v, 1);
        }
            /* New aggregate tag: pack all 0x30..0x37 in one shot */
        case T_INPUTS:
        {
            uint8_t in = HAL_get_inputs();
            uint8_t outm = HAL_get_outputs();
            uint16_t m10 = HAL_get_moisture_tenths();
            uint8_t alm = HAL_get_alarm_code();
            int8_t rssi = HAL_get_rssi_dbm();
            uint32_t ms = HAL_get_uptime_ms();
            uint32_t sec = ms / 1000;

            if (!put_tlv(out, cap, idx, T_INPUTS, &in, 1)) return false;
            if (!put_tlv(out, cap, idx, T_OUTPUTS, &outm, 1)) return false;
            if (!put_tlv_u16(out, cap, idx, T_MOIST_T10, m10)) return false;
            if (!put_tlv(out, cap, idx, T_ALARM_CODE, &alm, 1)) return false;
            if (!put_tlv(out, cap, idx, T_RSSI_DBM, &rssi, 1)) return false;
            if (!put_tlv_u32(out, cap, idx, T_UPTIME_MS, ms)) return false;
            if (!put_tlv_u32(out, cap, idx, T_UPTIME_S, sec)) return false;
            return true;
        }
        default:
            /* Unknown tag? ignore gracefully */
            return true;
    }
}

///* Apply TLVs for SET command */
//static bool tlv_apply(const uint8_t* p, size_t plen, uint8_t* st_out) {
//    if (plen < 2) {
//        *st_out = ST_BAD_TLV;
//        return false;
//    }
//    uint8_t tag = p[0], len = p[1];
//    if (plen < (size_t) (2 + len)) {
//        *st_out = ST_BAD_TLV;
//        return false;
//    }
//
//    switch (tag) {
//        case T_LED_STATE:
//            if (len != 1) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            led_set(p[2] ? true : false);
//            *st_out = ST_OK;
//            return true;
//        case T_SMS_EN:
//            if (len != 1) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            handle_sms_enable_cmd(p[2] ? 1u : 0u);
//            *st_out = ST_OK;
//            return true;
//        case T_PHONEBOOK_SET:
//        {
//            /* p[2] = slot (0..15); p+3..p+len-1 = ASCII number */
//            if (len < 2) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            uint8_t slot = p[2];
//            if (slot >= 16) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//
//            // --- DEBUG: print incoming SET TLV ---
//            UART3_WriteString33("T_PHONEBOOK_SET TLV received: slot=");
//            uart3_write_hex(slot);
//            UART3_WriteString33(" len=");
//            uart3_write_hex(len);
//            UART3_WriteString33(" data=\"");
//            for (uint8_t i = 0; i < (uint8_t) (len - 1); i++) {
//                char c = (char) p[3 + i];
//                // crude: send each char
//
//                UART3_WriteString33((char[]) {
//                    c, 0
//                });
//            }
//            UART3_WriteString33("\"\r\n");
//            // -------------------------------------
//
//            /* copy number (ensure zero-termination and max 23 chars) */
//            char num[24] = {0};
//            size_t copylen = len - 1;
//            if (copylen > 23) copylen = 23;
//            memcpy(num, &p[3], copylen);
//            phonebook_set_number(slot, num);
//
//            UART3_WriteString33("phonebook_set_number done\r\n");
//
//            *st_out = ST_OK;
//            return true;
//        }
//        case T_PHONEBOOK_LIST:
//        {
//            //debug
//            UART3_WriteString33("T_PHONEBOOK_LIST received, calling send_phonebook_list()\r\n");
//            /* no payload; respond to app with a TLV listing the numbers */
//            send_phonebook_list();
//            *st_out = ST_OK;
//            return true;
//        }
//        case T_PHONEBOOK_DEF:
//        {
//            if (len != 1) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            uint8_t slot = p[2];
//            if (slot >= 16) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            phonebook_set_default(slot);
//            *st_out = ST_OK;
//            return true;
//        }
//
//        case T_UART1_BAUD:
//        {
//            if (len != 4) {
//                *st_out = ST_BAD_TLV;
//                return false;
//            }
//            uint32_t baud = (uint32_t) p[2]
//                    | ((uint32_t) p[3] << 8)
//                    | ((uint32_t) p[4] << 16)
//                    | ((uint32_t) p[5] << 24);
//            if (baud < 1200 || baud > 921600) {
//                *st_out = ST_APPLY_FAIL;
//                return false;
//            }
//            UART_SERIAL_SETUP s = {.baudRate = baud, .dataWidth = UART_DATA_8_BIT,
//                .parity = UART_PARITY_NONE, .stopBits = UART_STOP_1_BIT};
//            bool ok = UART1_SerialSetup(&s, PBCLK_Hz()*2);
//            *st_out = ok ? ST_OK : ST_APPLY_FAIL;
//            return ok;
//        }
//        default:
//            *st_out = ST_BAD_TLV;
//            return false;
//    }
//}
//
///* Main handler for each received payload from ESP32 */
//void Esp_HandleFrame(const uint8_t* payload, size_t len) {
//    //Debug
//    UART3_WriteString33("ESP->PIC32 FRAME RECEIVED: ");
//    for (int i = 0; i < len; i++) {
//        uart3_write_hex(payload[i]);
//        UART3_WriteString33(" ");
//    }
//    UART3_WriteString33("\r\n");
//
//    //Debug
//
//    if (len == 0) return;
//    uint8_t op = payload[0];
//    const uint8_t* q = payload + 1;
//    size_t rem = (len > 0) ? (len - 1) : 0;
//
//    uint8_t rsp[320];
//    size_t ri = 0;
//    uint8_t status = ST_OK;
//    uint8_t rop = op | 0x80; // reply opcode
//
//    rsp[ri++] = rop;
//    rsp[ri++] = ST_OK; // placeholder
//
//    switch (op) {
//        case OPC_PING:
//            break; // no TLVs
//        case OPC_ECHO:
//            if (rem <= 255) {
//                if (!put_tlv(rsp, sizeof (rsp), &ri, 0xEE, q, (uint8_t) rem)) status = ST_BAD_LEN;
//            } else status = ST_BAD_LEN;
//            break;
//        case OPC_SET:
//        {
//            // Check for ALARMCTL sub-command
//            if (rem >= 1 && q[0] == SUB_ALARMCTL) {
//                q++;
//                rem--;
//                bool did = false;
//                while (rem >= 2) {
//                    uint8_t l = q[1];
//                    if (rem < (size_t) (2 + l)) {
//                        status = ST_BAD_TLV;
//                        break;
//                    }
//                    if (q[0] == T_ALARM_RESET && l == 1 && q[2] == 1) {
//                        HAL_reset_alarm();
//                        did = true;
//                    }
//                    rem -= (2 + l);
//                    q += (2 + l);
//                }
//                if (status == ST_OK && did) {
//                    uint8_t one = 1;
//                    (void) put_tlv(rsp, sizeof (rsp), &ri, T_ACK, &one, 1); // A0 01 01
//                } else if (status == ST_OK && !did) {
//                    status = ST_BAD_TLV;
//                }
//                break;
//            }
//
//            // Skip SUB_PHONEBOOK (0x22) if present
//            //            if (rem >= 1 && q[0] == 0x22) {
//            //                q++;
//            //                rem--;
//            //            }
//
//            // Legacy SET: treat TLVs directly
//            uint8_t last_led = 0xFF;
//            uint8_t last_sms = 0xFF;
//
//            while (rem >= 2) {
//                uint8_t l = q[1];
//                if (rem < (size_t) (2 + l)) {
//                    status = ST_BAD_TLV;
//                    break;
//                }
//
//                if (q[0] == T_LED_STATE && l == 1) last_led = q[2];
//                if (q[0] == T_SMS_EN && l == 1) last_sms = q[2];
//
//                uint8_t st_each = ST_OK;
//                bool ok = tlv_apply(q, 2 + l, &st_each);
//                if (!ok && status == ST_OK) status = st_each;
//                q += 2 + l;
//                rem -= 2 + l;
//            }
//            if (status == ST_OK) {
//                if (last_led != 0xFF) (void)put_tlv(rsp, sizeof rsp, &ri, T_ACK, &last_led, 1);
//                if (last_sms != 0xFF) (void)put_tlv(rsp, sizeof rsp, &ri, T_ACK, &last_sms, 1);
//            }
//            break;
//        }
//        case OPC_GET:
//        {
//            while (rem >= 2) {
//                uint8_t tag = q[0], l = q[1];
//                if (rem < (size_t) (2 + l)) {
//                    status = ST_BAD_TLV;
//                    break;
//                }
//                if (!tlv_get_reply(tag, rsp, sizeof (rsp), &ri)) {
//                    status = ST_BAD_LEN;
//                    break;
//                }
//                q += 2 + l;
//                rem -= 2 + l;
//            }
//            if (rem != 0 && status == ST_OK) status = ST_BAD_TLV;
//            break;
//        }
//        default:
//            status = ST_UNKNOWN_OP;
//            break;
//    }
//    rsp[1] = status;
//    ESP32_SendFrame(rsp, ri);
//}



//Test replacment

/* Apply TLVs for SET command */
static bool tlv_apply(const uint8_t* p, size_t plen, uint8_t* st_out) {
    if (plen < 2) {
        *st_out = ST_BAD_TLV;
        return false;
    }
    uint8_t tag = p[0], len = p[1];
    if (plen < (size_t) (2 + len)) {
        *st_out = ST_BAD_TLV;
        return false;
    }

    switch (tag) {
        case T_LED_STATE:
            if (len != 1) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            led_set(p[2] ? true : false);
            *st_out = ST_OK;
            return true;

        case T_SMS_EN:
            if (len != 1) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            handle_sms_enable_cmd(p[2] ? 1u : 0u);
            *st_out = ST_OK;
            return true;

        case T_PHONEBOOK_SET:
        {
            /* p[2] = slot (0..15); p+3..p+len-1 = ASCII number */
            if (len < 2) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            uint8_t slot = p[2];
            if (slot >= 16) {
                *st_out = ST_BAD_TLV;
                return false;
            }

            // --- DEBUG: print incoming SET TLV ---
            UART3_WriteString33("T_PHONEBOOK_SET TLV received: slot=");
            uart3_write_hex(slot);
            UART3_WriteString33(" len=");
            uart3_write_hex(len);
            UART3_WriteString33(" data=\"");
            for (uint8_t i = 0; i < (uint8_t) (len - 1); i++) {
                char c = (char) p[3 + i];

                UART3_WriteString33((char[]) {
                    c, 0});
            }
            UART3_WriteString33("\"\r\n");
            // -------------------------------------

            /* copy number (ensure zero-termination and max 23 chars) */
            char num[24] = {0};
            size_t copylen = len - 1;
            if (copylen > 23) copylen = 23;
            memcpy(num, &p[3], copylen);

            phonebook_set_number(slot, num);

            UART3_WriteString33("phonebook_set_number done\r\n");

            *st_out = ST_OK;
            return true;
        }

        case T_PHONEBOOK_LIST:
        {
            UART3_WriteString33("T_PHONEBOOK_LIST received, calling send_phonebook_list()\r\n");
            /* no payload; respond to app with a TLV listing the numbers */
            send_phonebook_list();
            *st_out = ST_OK;
            return true;
        }

        case T_PHONEBOOK_DEF:
        {
            if (len != 1) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            uint8_t slot = p[2];
            if (slot >= 16) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            phonebook_set_default(slot);
            *st_out = ST_OK;
            return true;
        }

        case T_UART1_BAUD:
        {
            if (len != 4) {
                *st_out = ST_BAD_TLV;
                return false;
            }
            uint32_t baud = (uint32_t) p[2]
                    | ((uint32_t) p[3] << 8)
                    | ((uint32_t) p[4] << 16)
                    | ((uint32_t) p[5] << 24);
            if (baud < 1200 || baud > 921600) {
                *st_out = ST_APPLY_FAIL;
                return false;
            }
            UART_SERIAL_SETUP s = {
                .baudRate = baud,
                .dataWidth = UART_DATA_8_BIT,
                .parity = UART_PARITY_NONE,
                .stopBits = UART_STOP_1_BIT
            };
            bool ok = UART1_SerialSetup(&s, PBCLK_Hz() * 2);
            *st_out = ok ? ST_OK : ST_APPLY_FAIL;
            return ok;
        }

        default:
            *st_out = ST_BAD_TLV;
            return false;
    }
}




//Test

/* Main handler for each received payload from ESP32 */
void Esp_HandleFrame(const uint8_t* payload, size_t len) {
    // Debug: print raw frame
    UART3_WriteString33("ESP->PIC32 FRAME RECEIVED: ");
    for (size_t i = 0; i < len; i++) {
        uart3_write_hex(payload[i]);
        UART3_WriteString33(" ");
    }
    UART3_WriteString33("\r\n");

    if (len == 0) return;

    uint8_t op = payload[0];
    const uint8_t* q = payload + 1;
    size_t rem = (len > 0) ? (len - 1) : 0;

    uint8_t rsp[320];
    size_t ri = 0;
    uint8_t status = ST_OK;
    uint8_t rop = op | 0x80; // reply opcode

    rsp[ri++] = rop;
    rsp[ri++] = ST_OK; // placeholder for status

    switch (op) {
        case OPC_PING:
            // No TLVs, just reply with opcode + status = OK
            break;

        case OPC_ECHO:
            if (rem <= 255) {
                if (!put_tlv(rsp, sizeof (rsp), &ri, 0xEE, q, (uint8_t) rem)) {
                    status = ST_BAD_LEN;
                }
            } else {
                status = ST_BAD_LEN;
            }
            break;


        case OPC_SET:
        {
            // ? TEMP: Disable ALARMCTL subcommand because 0x40 is used by phonebook.
            // We treat all TLVs in the payload directly via tlv_apply().

            uint8_t last_led = 0xFF;
            uint8_t last_sms = 0xFF;

            while (rem >= 2) {
                uint8_t l = q[1];
                if (rem < (size_t) (2 + l)) {
                    status = ST_BAD_TLV;
                    break;
                }

                if (q[0] == T_LED_STATE && l == 1) last_led = q[2];
                if (q[0] == T_SMS_EN && l == 1) last_sms = q[2];

                uint8_t st_each = ST_OK;
                bool ok = tlv_apply(q, 2 + l, &st_each);
                if (!ok && status == ST_OK) status = st_each;

                q += 2 + l;
                rem -= 2 + l;
            }

            if (status == ST_OK) {
                if (last_led != 0xFF) (void)put_tlv(rsp, sizeof (rsp), &ri, T_ACK, &last_led, 1);
                if (last_sms != 0xFF) (void)put_tlv(rsp, sizeof (rsp), &ri, T_ACK, &last_sms, 1);
            }
            break;
        }

            //        case OPC_SET:
            //        {
            //            // Check for ALARMCTL sub-command
            //            if (rem >= 1 && q[0] == SUB_ALARMCTL) {
            //                q++;
            //                rem--;
            //                bool did = false;
            //                while (rem >= 2) {
            //                    uint8_t l = q[1];
            //                    if (rem < (size_t)(2 + l)) {
            //                        status = ST_BAD_TLV;
            //                        break;
            //                    }
            //                    if (q[0] == T_ALARM_RESET && l == 1 && q[2] == 1) {
            //                        HAL_reset_alarm();
            //                        did = true;
            //                    }
            //                    rem -= (2 + l);
            //                    q   += (2 + l);
            //                }
            //                if (status == ST_OK && did) {
            //                    uint8_t one = 1;
            //                    (void)put_tlv(rsp, sizeof(rsp), &ri, T_ACK, &one, 1); // A0 01 01
            //                } else if (status == ST_OK && !did) {
            //                    status = ST_BAD_TLV;
            //                }
            //                break;
            //            }
            //
            //            // Legacy SET: treat TLVs directly
            //            uint8_t last_led = 0xFF;
            //            uint8_t last_sms = 0xFF;
            //
            //            while (rem >= 2) {
            //                uint8_t l = q[1];
            //                if (rem < (size_t)(2 + l)) {
            //                    status = ST_BAD_TLV;
            //                    break;
            //                }
            //
            //                if (q[0] == T_LED_STATE && l == 1) last_led = q[2];
            //                if (q[0] == T_SMS_EN   && l == 1) last_sms = q[2];
            //
            //                uint8_t st_each = ST_OK;
            //                bool ok = tlv_apply(q, 2 + l, &st_each);
            //                if (!ok && status == ST_OK) status = st_each;
            //
            //                q   += 2 + l;
            //                rem -= 2 + l;
            //            }
            //
            //            if (status == ST_OK) {
            //                if (last_led != 0xFF) (void)put_tlv(rsp, sizeof(rsp), &ri, T_ACK, &last_led, 1);
            //                if (last_sms != 0xFF) (void)put_tlv(rsp, sizeof(rsp), &ri, T_ACK, &last_sms, 1);
            //            }
            //            break;
            //        }

        case OPC_GET:
        {
            while (rem >= 2) {
                uint8_t tag = q[0], l = q[1];
                if (rem < (size_t) (2 + l)) {
                    status = ST_BAD_TLV;
                    break;
                }
                if (!tlv_get_reply(tag, rsp, sizeof (rsp), &ri)) {
                    status = ST_BAD_LEN;
                    break;
                }
                q += 2 + l;
                rem -= 2 + l;
            }
            if (rem != 0 && status == ST_OK) {
                status = ST_BAD_TLV;
            }
            break;
        }

        default:
            status = ST_UNKNOWN_OP;
            break;
    }

    rsp[1] = status;
    ESP32_SendFrame(rsp, ri);
}

