#ifndef ESP32_H
#define ESP32_H

#include <stdint.h>
#include <stdbool.h>

/** Initialize ESP32 AT?service (clear buffers). */
void Esp32_Init(void);

/** Feed raw bytes from UART1 into our line parser. */
void handleEsp32Response(const uint8_t *buf, int len);

/** Returns true if one or more complete lines are waiting. */
bool esp32LineReady(void);

/**
 * Retrieve the oldest complete line (CR?LF stripped).
 * Returns pointer to internal buffer (valid until next call).
 */
const char* esp32GetLine(void);

/** Send an AT command (appends "\r\n"). */
void esp32SendCommand(const char *cmd);

#endif // ESP32_H
