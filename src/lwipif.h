#ifndef LWIPIF_H
#define LWIPIF_H

#include <stdint.h>

/** Initialize lwIP stack, network interface, DHCP/static IP, etc. */
void Network_Init(void);

/** Call periodically (e.g. every 5?ms) to service lwIP timers and input. */
void Network_PeriodicTasks(void);

/**
 * Send a TCP packet (simplified, one?shot).
 * ip: dotted string, port: destination port.
 * Returns 0 on success, negative on error.
 */
int Network_Send(const uint8_t *data, int len, const char *ip, uint16_t port);

/**
 * Non?blocking receive: copy up to maxlen bytes into buf.
 * Returns number of bytes read, or 0 if none.
 */
int Network_Receive(uint8_t *buf, int maxlen);

#endif // LWIPIF_H
