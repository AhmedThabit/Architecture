#include "telit.h"
#include "peripheral/uart/plib_uart3.h"  // Harmony/PLIB UART3 API
#include "peripheral/uart/plib_uart1.h"
#include "peripheral/uart/plib_uart2.h"   
#include <string.h>         // for memset(), strncpy(), strlen()

#define TELIT_MAX_LINES    8
#define TELIT_MAX_LINE_LEN 128

static char lineBuf[TELIT_MAX_LINE_LEN];
static int  linePos;

// ring buffer of complete lines
static char lines[TELIT_MAX_LINES][TELIT_MAX_LINE_LEN];
static int  lineHead, lineTail;

void Telit_Init(void) {
    linePos  = 0;
    lineHead = lineTail = 0;
    memset(lines, 0, sizeof(lines));
}

void handleTelitResponse(const uint8_t *buf, int len) {
    for (int i = 0; i < len; i++) {
        char c = (char)buf[i];
        if (c == '\r') {
            // ignore
        }
        else if (c == '\n') {
            if (linePos > 0) {
                lineBuf[linePos] = '\0';
                int next = (lineHead + 1) % TELIT_MAX_LINES;
                if (next != lineTail) {
                    strncpy(lines[lineHead], lineBuf, TELIT_MAX_LINE_LEN);
                    lineHead = next;
                }
                linePos = 0;
            }
        }
        else if (linePos < TELIT_MAX_LINE_LEN - 1) {
            lineBuf[linePos++] = c;
        }
    }
}

bool telitLineReady(void) {
    return (lineHead != lineTail);
}

const char* telitGetLine(void) {
    if (lineHead == lineTail) {
        return NULL;
    }
    const char *ln = lines[lineTail];
    lineTail = (lineTail + 1) % TELIT_MAX_LINES;
    return ln;
}

void telitSendCommand(const char *cmd) {
    size_t len = strlen(cmd);
    // cast away const so we can call the PLIB API
    UART3_Write((uint8_t*)cmd, (size_t)len);
    UART3_Write((uint8_t*)"\r\n", 2);
}
