#include "esp32.h"

#include "peripheral/uart/plib_uart1.h"
#include "peripheral/uart/plib_uart2.h"
#include "peripheral/uart/plib_uart3.h"
#include <string.h>

#define ESP32_MAX_LINES    8
#define ESP32_MAX_LINE_LEN 128

static char lineBuf[ESP32_MAX_LINE_LEN];
static int  linePos;

static char lines[ESP32_MAX_LINES][ESP32_MAX_LINE_LEN];
static int  head, tail;

void Esp32_Init(void) {
    linePos = 0;
    head = tail = 0;
}

void handleEsp32Response(const uint8_t *buf, int len) {
    for (int i = 0; i < len; i++) {
        char c = buf[i];
        if (c == '\r') {
            // skip
        }
        else if (c == '\n') {
            if (linePos > 0) {
                lineBuf[linePos] = '\0';
                int next = (head + 1) % ESP32_MAX_LINES;
                if (next != tail) {
                    strncpy(lines[head], lineBuf, ESP32_MAX_LINE_LEN);
                    head = next;
                }
                linePos = 0;
            }
        }
        else if (linePos < ESP32_MAX_LINE_LEN - 1) {
            lineBuf[linePos++] = c;
        }
    }
}

bool esp32LineReady(void) {
    return (head != tail);
}

const char* esp32GetLine(void) {
    if (head == tail) return NULL;
    const char *ln = lines[tail];
    tail = (tail + 1) % ESP32_MAX_LINES;
    return ln;
}

void esp32SendCommand(const char *cmd) {
    int len = strlen(cmd);
    UART1_Write((const uint8_t*)cmd, len);
    UART1_Write((const uint8_t*)"\r\n", 2);
}
