#ifndef TELIT_H
#define TELIT_H

#include <stdint.h>   // for uint8_t
#include <stdbool.h>  // for bool

/** Initialize Telit AT?service (clear buffers). */
void Telit_Init(void);

/** Feed raw bytes from UART3 into our line parser. */
void handleTelitResponse(const uint8_t *buf, int len);

/** Returns true if one or more complete lines are waiting. */
bool telitLineReady(void);

/**
 * Retrieve the oldest complete line (CR?LF stripped).
 * Returns pointer to internal buffer (valid until next call).
 */
const char* telitGetLine(void);

/** Send an AT command (appends "\r\n"). */
void telitSendCommand(const char *cmd);

#endif // TELIT_H
