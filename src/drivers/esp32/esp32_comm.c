// esp32_comm.c
// esp32_comm.c
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "peripheral/uart/plib_uart1.h"
#include "esp32_comm.h"

#define ESP32_ASCII_TEST 0   // 1 = allow RealTerm ASCII testing; 0 = binary only

// ================== CONFIG ==================
#ifndef ESP32_ASCII_TEST
#define ESP32_ASCII_TEST 1   // 1 = allow RealTerm ASCII testing; 0 = binary only
#endif

// get an ASCII ?OK? back in RealTerm
#if ESP32_ASCII_TEST

static void ascii_ack(const char* s) {
    UART1_Write((uint8_t*) s, strlen(s));
}
#endif

#define RX_BUF_MAX     512
#define PAYLOAD_MAX    256
#define IDLE_TIMEOUT_MS 20

// Opcodes / TLVs used to fabricate payloads from ASCII commands
#define OPC_SET          0x02
#define T_LED_STATE      0x10

// ============================================

static int hexval_char(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}


static volatile bool rx_flag = false;
static ESP32_FrameHandler app_cb = 0;

static uint16_t crc16_ccitt(const uint8_t* d, size_t n) {
    uint16_t c = 0xFFFF;
    while (n--) {
        c ^= (uint16_t) (*d++) << 8;
        for (int i = 0; i < 8; i++) c = (c & 0x8000) ? (c << 1)^0x1021 : (c << 1);
    }
    return c;
}

/* ISR callback: set a flag only */
static void uart1_rx_cb(UART_EVENT ev, uintptr_t ctx) {
    (void) ctx;
    if (ev == UART_EVENT_READ_THRESHOLD_REACHED ||
            ev == UART_EVENT_READ_BUFFER_FULL ||
            ev == UART_EVENT_READ_ERROR) {
        rx_flag = true;
    }
}

void ESP32_UartInit(void) {
    UART1_ReadThresholdSet(1);
    UART1_ReadNotificationEnable(true, true);
    UART1_ReadCallbackRegister(uart1_rx_cb, 0);
}
//
//bool ESP32_TakeRxFlag(void)
//{
//    bool v = esp_rx_ready;
//    esp_rx_ready = false;
//    return v;
//}

void ESP32_RegisterFrameHandler(ESP32_FrameHandler cb) {
    app_cb = cb;
}

bool ESP32_SendFrame(const uint8_t* p, size_t len) {
    if (len == 0 || len > PAYLOAD_MAX) return false;
    uint8_t hdr[2] = {(uint8_t) (len & 0xFF), (uint8_t) (len >> 8)};
    uint16_t c = crc16_ccitt(p, len);
    uint8_t ftr[2] = {(uint8_t) (c & 0xFF), (uint8_t) (c >> 8)};

    size_t n = 0;
    n += UART1_Write(hdr, sizeof hdr);
    n += UART1_Write((uint8_t*) p, len);
    n += UART1_Write(ftr, sizeof ftr);
    return n == (2 + len + 2);
}

/* Parser state */
static enum {
    RX_LEN0, RX_LEN1, RX_PAY, RX_CRC0, RX_CRC1
} st = RX_LEN0;
static uint16_t want_len = 0, got = 0, rx_crc = 0;
static uint8_t frame[PAYLOAD_MAX];
static uint32_t last_ms = 0;




#if ESP32_ASCII_TEST
/* ===== Helpers to support RealTerm ASCII tests ===== */

// Feed raw bytes into the same binary state machine

static void feed_bytes_to_parser(const uint8_t* data, size_t n) {
    extern volatile uint32_t g_ms_ticks;
    for (size_t i = 0; i < n; i++) {
        uint8_t b = data[i];
        last_ms = g_ms_ticks;
        switch (st) {
            case RX_LEN0: want_len = b;
                st = RX_LEN1;
                break;
            case RX_LEN1:
                want_len |= (uint16_t) b << 8;
                if (want_len == 0 || want_len > PAYLOAD_MAX) {
                    st = RX_LEN0;
                    break;
                }
                got = 0;
                st = RX_PAY;
                break;
            case RX_PAY:
                frame[got++] = b;
                if (got == want_len) st = RX_CRC0;
                break;
            case RX_CRC0: rx_crc = b;
                st = RX_CRC1;
                break;
            case RX_CRC1:
            {
                rx_crc |= (uint16_t) b << 8;
                uint16_t calc = crc16_ccitt(frame, want_len);
                if (app_cb && calc == rx_crc) app_cb(frame, want_len);
                st = RX_LEN0;
            }
            break;
        }
    }
}


// Parse "HEX AA BB CC ..." into bytes

static bool parse_hex_line(const char* s, uint8_t* out, size_t* outLen, size_t cap) {
    /* skip leading spaces and optional "HEX" prefix (case-insensitive) */
    while (*s == ' ' || *s == '\t') s++;
    if ((s[0] == 'H' || s[0] == 'h') && (s[1] == 'E' || s[1] == 'e') && (s[2] == 'X' || s[2] == 'x')) {
        s += 3;
    }
    while (*s == ' ' || *s == '\t' || *s == ':') s++;

    size_t n = 0;
    while (*s) {
        /* skip separators */
        while (*s == ' ' || *s == '\t' || *s == ',') s++;
        if (!*s) break;

        int hi = hexval_char(*s++);
        if (hi < 0) return false;
        while (*s == ' ') s++;
        if (!*s) return false;
        int lo = hexval_char(*s++);
        if (lo < 0) return false;

        if (n >= cap) return false;
        out[n++] = (uint8_t) ((hi << 4) | lo);

        while (*s == ' ' || *s == '\t') s++;
    }
    *outLen = n;
    return (n > 0);
}

// Handle one complete ASCII line:
//  - "HEX ..." ? parse to bytes and feed the binary parser
//  - "LED=0/1" ? fabricate a SET TLV payload and call app_cb

static void handle_ascii_line(const char* line) {
    /* try HEX first */
    uint8_t bytes[320];
    size_t nb = 0;
    if (parse_hex_line(line, bytes, &nb, sizeof (bytes))) {
        feed_bytes_to_parser(bytes, nb);
        return;
    }

    /* case-insensitive "LED=" */
    if ((line[0] == 'L' || line[0] == 'l') &&
            (line[1] == 'E' || line[1] == 'e') &&
            (line[2] == 'D' || line[2] == 'd') &&
            line[3] == '=') {
        uint8_t v = (line[4] == '1') ? 1u : 0u;
        uint8_t payload[4] = {OPC_SET, T_LED_STATE, 0x01, v};
        if (app_cb) app_cb(payload, sizeof payload);
#if ESP32_ASCII_TEST
        ascii_ack(v ? "OK LED=1\r\n" : "OK LED=0\r\n");
#endif
        return;
    }

    /* add more ASCII commands here if needed */
}

#endif /* ESP32_ASCII_TEST */

/* ================== Public poll ================== */
void ESP32_Poll(void) {
// UART1_Write((uint8_t*)"BIN\n", 4);   // remove after confirming   
#if ESP32_ASCII_TEST
    // ASCII test mode: collect UART1 into lines and handle
    uint8_t buf[64];
    size_t n = UART1_Read(buf, sizeof buf);

    static char line[256];
    static size_t L = 0;

    for (size_t i = 0; i < n; i++) {
        char c = (char) buf[i];
        if (c == '\r' || c == '\n') {
            if (L > 0) {
                line[L] = '\0';
                handle_ascii_line(line);
                L = 0;
            }
            // swallow multiple CR/LF
        } else {
            if (L < sizeof (line) - 1) line[L++] = c;
            else L = 0; // overflow -> reset
        }
    }
    // no need for rx_flag in ASCII mode
    (void) rx_flag;

#else
    // Binary mode (your original parser)
    if (!rx_flag && st == RX_LEN0) return; // nothing pending
    rx_flag = false;

    uint8_t chunk[64];
    size_t n = UART1_Read(chunk, sizeof chunk);
    extern volatile uint32_t g_ms_ticks;

    for (size_t i = 0; i < n; i++) {
        uint8_t b = chunk[i];
        last_ms = g_ms_ticks;

        switch (st) {
            case RX_LEN0: want_len = b;
                st = RX_LEN1;
                break;
            case RX_LEN1:
                want_len |= (uint16_t) b << 8;
                if (want_len == 0 || want_len > PAYLOAD_MAX) {
                    st = RX_LEN0;
                    break;
                }
                got = 0;
                st = RX_PAY;
                break;
            case RX_PAY:
                frame[got++] = b;
                if (got == want_len) {
                    st = RX_CRC0;
                }
                break;
            case RX_CRC0: rx_crc = b;
                st = RX_CRC1;
                break;
            case RX_CRC1:
                rx_crc |= (uint16_t) b << 8;
                if (app_cb && rx_crc == crc16_ccitt(frame, want_len))
                    app_cb(frame, want_len); // application handler
                st = RX_LEN0;
                break;
        }
    }

    // idle timeout to drop a stuck partial frame
    if (st != RX_LEN0) {
        extern volatile uint32_t g_ms_ticks;
        if ((uint32_t) (g_ms_ticks - last_ms) >= IDLE_TIMEOUT_MS) st = RX_LEN0;
    }
#endif
}

