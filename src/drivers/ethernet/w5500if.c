#include "w5500if.h"
#include "config/default/peripheral/spi/spi_master/plib_spi1_master.h"    // MPLAB Harmony SPI1 driver
#include "definitions.h"     // your HAL GPIO for CS/INT
#include <string.h>       // for memcpy, memset
#include <stdint.h>
#include <stdbool.h>
//------------------------------------------------------------------------------
// Pin definitions (per your board)
//------------------------------------------------------------------------------

#define W5500_CS_PORT   3   // PORTD = index 3
#define W5500_CS_PIN    2   // RD2

#define W5500_INT_PORT  1   // PORTB = index 1
#define W5500_INT_PIN   2   // RB2

//------------------------------------------------------------------------------
// W5500 common register addresses (per datasheet)
//------------------------------------------------------------------------------

#define W5500_COMMON_BLOCK  0x08
#define W5500_GAR           0x0001  // Gateway
#define W5500_SUBR          0x0005  // Subnet mask
#define W5500_SHAR          0x0009  // MAC address
#define W5500_SIPR          0x000F  // IP address

//------------------------------------------------------------------------------
// SPI + CS helpers
//------------------------------------------------------------------------------

static inline void W5500_Select(void)   { HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, false); }
static inline void W5500_Deselect(void) { HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, true); }

// Exchange a single byte over SPI1
static uint8_t SPI_Exchange(uint8_t b) {
    return SPI1_Exchange8bit(b);
}

//------------------------------------------------------------------------------
// Low?level register read/write
//------------------------------------------------------------------------------

static void W5500_WriteReg(uint16_t addr, uint8_t block, const uint8_t *buf, uint16_t len) {
    W5500_Select();
    SPI_Exchange(addr >> 8);
    SPI_Exchange(addr & 0xFF);
    // Control byte: [Bsb(3) | R/W(1)=1 | OM(2)=len>1?2:0]
    SPI_Exchange((block << 3) | 0x04 | ((len > 1)?0x02:0x00));
    for (uint16_t i = 0; i < len; i++) {
        SPI_Exchange(buf[i]);
    }
    W5500_Deselect();
}

static void W5500_ReadReg(uint16_t addr, uint8_t block, uint8_t *buf, uint16_t len) {
    W5500_Select();
    SPI_Exchange(addr >> 8);
    SPI_Exchange(addr & 0xFF);
    // Control byte: [Bsb(3) | R/W(1)=0 | OM(2)=len>1?2:0]
    SPI_Exchange((block << 3) | 0x00 | ((len > 1)?0x02:0x00));
    for (uint16_t i = 0; i < len; i++) {
        buf[i] = SPI_Exchange(0xFF);
    }
    W5500_Deselect();
}

//------------------------------------------------------------------------------
// Public API
//------------------------------------------------------------------------------

void W5500IF_Init(const uint8_t mac[6],
                  const uint8_t ip[4],
                  const uint8_t gw[4],
                  const uint8_t sn[4])
{
    // 1) Init SPI1
    SPI1_Initialize();

    // 2) Configure CS pin (RD2) as output, deselect
//    GPIO_ConfigPin(W5500_CS_PORT, W5500_CS_PIN, false);
    W5500_Deselect();

    // 3) Configure INT pin (RB2) as input
//    GPIO_ConfigPin(W5500_INT_PORT, W5500_INT_PIN, true);

    // 4) Program network registers
    W5500_WriteReg(W5500_GAR,  W5500_COMMON_BLOCK, gw,  4);
    W5500_WriteReg(W5500_SUBR, W5500_COMMON_BLOCK, sn,  4);
    W5500_WriteReg(W5500_SHAR, W5500_COMMON_BLOCK, mac, 6);
    W5500_WriteReg(W5500_SIPR, W5500_COMMON_BLOCK, ip,  4);

    // (Optional) verify by reading back?
}

bool W5500IF_SocketOpen(uint8_t sock, uint8_t protocol, uint16_t port) {
    uint16_t base = 0x4000 + sock * 0x0100;
    // Set Mode register (Sn_MR)
    W5500_WriteReg(base + 0x0000, sock, &protocol, 1);
    // Set Port (Sn_PORT)
    uint8_t port_buf[2] = { (uint8_t)(port >> 8), (uint8_t)(port & 0xFF) };
    W5500_WriteReg(base + 0x0004, sock, port_buf, 2);
    // Send OPEN command (Sn_CR = 0x01)
    uint8_t cmd = 0x01;
    W5500_WriteReg(base + 0x0001, sock, &cmd, 1);
    return true;
}

int W5500IF_SocketSend(uint8_t sock, const uint8_t *buf, uint16_t len) {
    // TODO: implement proper TX sequence (check FSR, write pointer, CR=SEND)
    // For now, pretend success:
    (void)sock; (void)buf; (void)len;
    return (int)len;
}

int W5500IF_SocketRecv(uint8_t sock, uint8_t *buf, uint16_t bufsize) {
    // TODO: implement proper RX sequence (check RSR, read pointer, CR=RECV)
    (void)sock; (void)buf; (void)bufsize;
    return 0;
}

void W5500IF_SocketClose(uint8_t sock) {
    uint16_t base = 0x4000 + sock * 0x0100;
    uint8_t cmd = 0x10;  // CLOSE
    W5500_WriteReg(base + 0x0001, sock, &cmd, 1);
}
