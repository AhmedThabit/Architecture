#ifndef W5500IF_H
#define W5500IF_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  Initialize the W5500:
 *         ? Initializes SPI1 peripheral
 *         ? Configures CS (RD2) and INT (RB2) pins
 *         ? Programs MAC, IP, Gateway, Subnet Mask
 *
 * @param  mac  6?byte MAC address
 * @param  ip   4?byte IPv4 address
 * @param  gw   4?byte gateway address
 * @param  sn   4?byte subnet mask
 */
void W5500IF_Init(const uint8_t mac[6],
                  const uint8_t ip[4],
                  const uint8_t gw[4],
                  const uint8_t sn[4]);

/**
 * @brief  Open a W5500 socket.
 *
 * @param  sock      Socket number (0?7)
 * @param  protocol  0x01 = TCP, 0x02 = UDP
 * @param  port      Local port to bind
 * @return true on success, false on failure
 */
bool W5500IF_SocketOpen(uint8_t sock, uint8_t protocol, uint16_t port);

/**
 * @brief  Send data over an open socket.
 *
 * @param  sock  Socket number
 * @param  buf   Pointer to data buffer
 * @param  len   Number of bytes to send
 * @return Number of bytes sent, or <0 on error
 */
int  W5500IF_SocketSend(uint8_t sock, const uint8_t *buf, uint16_t len);

/**
 * @brief  Receive data from an open socket (non?blocking).
 *
 * @param  sock     Socket number
 * @param  buf      Buffer to fill
 * @param  bufsize  Max bytes to read
 * @return Number of bytes read (>=0)
 */
int  W5500IF_SocketRecv(uint8_t sock, uint8_t *buf, uint16_t bufsize);

/**
 * @brief  Close an open socket.
 *
 * @param  sock  Socket number
 */
void W5500IF_SocketClose(uint8_t sock);

#endif // W5500IF_H
