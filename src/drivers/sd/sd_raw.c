#include "sd_raw.h"
#include <string.h>
#include "peripheral/spi/spi_master/plib_spi3_master.h"
#include "peripheral/gpio/plib_gpio.h"

/* CS for SD */
#define SD_CS_HIGH() GPIO_RC11_SD_SS3_Set()
#define SD_CS_LOW()  GPIO_RC11_SD_SS3_Clear()

/* Dummy byte */
#define SPI_DUMMY 0xFF

/* SD commands (SPI mode) */
#define CMD0   (0)
#define CMD8   (8)
#define CMD17  (17)
#define CMD24  (24)
#define CMD55  (55)
#define ACMD41 (41)
#define CMD58  (58)
#define CMD16  (16)
#define CMD9   (9)

#define R1_IDLE  (0x01)
#define R1_OK    (0x00)

/* You already have msTicks */
extern volatile uint32_t msTicks;

static bool g_sdhc = false;

static void spi_set_hz(uint32_t hz)
{
    SPI_TRANSFER_SETUP s;
    s.clockFrequency = hz;
    s.clockPolarity  = 0; /* mode 0 */
    s.clockPhase     = 0;
    s.dataBits       = SPI_DATA_BITS_8;
    (void)SPI3_TransferSetup(&s, 0);
}

static uint8_t spi_xfer(uint8_t v)
{
    uint8_t r = 0xFF;
    SPI3_WriteRead(&v, 1, &r, 1);
    while (SPI3_IsBusy());
    return r;
}

static void spi_clock_idle(uint32_t nbytes)
{
    SD_CS_HIGH();
    for (uint32_t i = 0; i < nbytes; i++) (void)spi_xfer(SPI_DUMMY);
}

/* Wait for card not-busy (returns true if ready before timeout) */
static bool wait_not_busy(uint32_t timeout_ms)
{
    uint32_t t0 = msTicks;
    uint8_t b;
    do {
        b = spi_xfer(SPI_DUMMY);
        if (b == 0xFF) return true;
    } while ((msTicks - t0) < timeout_ms);
    return false;
}

static uint8_t send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    SD_CS_LOW();
    (void)wait_not_busy(5);

    spi_xfer(0x40 | cmd);
    spi_xfer((uint8_t)(arg >> 24));
    spi_xfer((uint8_t)(arg >> 16));
    spi_xfer((uint8_t)(arg >> 8));
    spi_xfer((uint8_t)(arg));
    spi_xfer(crc);

    /* response within 8 bytes */
    for (int i = 0; i < 8; i++) {
        uint8_t r = spi_xfer(SPI_DUMMY);
        if ((r & 0x80) == 0) return r;
    }
    return 0xFF;
}

static void end_cmd(void)
{
    SD_CS_HIGH();
    spi_xfer(SPI_DUMMY);
}

static bool read_r3_r7(uint8_t* ocr4)
{
    for (int i = 0; i < 4; i++) ocr4[i] = spi_xfer(SPI_DUMMY);
    return true;
}

bool SDRAW_Init(void)
{
    /* Ensure SD CS pin is output + high */
    ANSELCCLR = (1u << 11);
    GPIO_RC11_SD_SS3_OutputEnable();
    SD_CS_HIGH();

    /* Low speed for init (100-400k) */
    spi_set_hz(400000);

    /* 80+ clocks with CS high */
    spi_clock_idle(10);

    /* CMD0 -> IDLE */
    uint8_t r = send_cmd(CMD0, 0, 0x95);
    end_cmd();
    if (r != R1_IDLE) return false;

    /* CMD8 check */
    r = send_cmd(CMD8, 0x1AA, 0x87);
    uint8_t r7[4] = {0};
    if (r == R1_IDLE) {
        read_r3_r7(r7);
        end_cmd();
        /* must end with 0x1AA */
        if (r7[2] != 0x01 || r7[3] != 0xAA) return false;
    } else {
        end_cmd();
        /* older card: continue anyway */
    }

    /* ACMD41 loop */
    uint32_t t0 = msTicks;
    do {
        r = send_cmd(CMD55, 0, 0x01);
        end_cmd();
        if (r > 0x01) return false;

        r = send_cmd(ACMD41, 0x40000000, 0x01); /* HCS */
        end_cmd();
        if (r == 0x00) break;
    } while ((msTicks - t0) < 1000);

    if (r != 0x00) return false;

    /* CMD58 OCR */
    r = send_cmd(CMD58, 0, 0x01);
    uint8_t ocr[4];
    read_r3_r7(ocr);
    end_cmd();
    if (r != 0x00) return false;

    g_sdhc = (ocr[0] & 0x40) != 0; /* CCS */

    /* Set block length for SDSC */
    if (!g_sdhc) {
        r = send_cmd(CMD16, 512, 0x01);
        end_cmd();
        if (r != 0x00) return false;
    }

    /* Now go faster */
    spi_set_hz(5000000); /* start 5MHz; you can raise later */
    return true;
}

bool SDRAW_ReadSectors(uint32_t lba, uint8_t* buf, uint32_t count)
{
    if (!buf || count == 0) return false;

    while (count--) {
        uint32_t addr = g_sdhc ? lba : (lba * 512u);

        uint8_t r = send_cmd(CMD17, addr, 0x01);
        if (r != 0x00) { end_cmd(); return false; }

        /* Wait token 0xFE */
        uint32_t t0 = msTicks;
        uint8_t tok;
        do {
            tok = spi_xfer(SPI_DUMMY);
            if (tok == 0xFE) break;
        } while ((msTicks - t0) < 200);
        if (tok != 0xFE) { end_cmd(); return false; }

        for (int i = 0; i < 512; i++) buf[i] = spi_xfer(SPI_DUMMY);

        /* CRC */
        (void)spi_xfer(SPI_DUMMY);
        (void)spi_xfer(SPI_DUMMY);

        end_cmd();

        buf += 512;
        lba++;
    }
    return true;
}

bool SDRAW_WriteSectors(uint32_t lba, const uint8_t* buf, uint32_t count)
{
    if (!buf || count == 0) return false;

    while (count--) {
        uint32_t addr = g_sdhc ? lba : (lba * 512u);

        uint8_t r = send_cmd(CMD24, addr, 0x01);
        if (r != 0x00) { end_cmd(); return false; }

        spi_xfer(0xFE); /* start token */
        for (int i = 0; i < 512; i++) spi_xfer(buf[i]);
        spi_xfer(0xFF); /* CRC */
        spi_xfer(0xFF);

        uint8_t resp = spi_xfer(SPI_DUMMY);
        if ((resp & 0x1F) != 0x05) { end_cmd(); return false; }

        /* wait internal write done */
        if (!wait_not_busy(500)) { end_cmd(); return false; }

        end_cmd();

        buf += 512;
        lba++;
    }
    return true;
}

bool SDRAW_GetSectorCount(uint32_t* out_sectors)
{
    if (!out_sectors) return false;

    /* CMD9 read CSD */
    uint8_t r = send_cmd(CMD9, 0, 0x01);
    if (r != 0x00) { end_cmd(); return false; }

    uint32_t t0 = msTicks;
    uint8_t tok;
    do {
        tok = spi_xfer(SPI_DUMMY);
        if (tok == 0xFE) break;
    } while ((msTicks - t0) < 200);
    if (tok != 0xFE) { end_cmd(); return false; }

    uint8_t csd[16];
    for (int i = 0; i < 16; i++) csd[i] = spi_xfer(SPI_DUMMY);
    (void)spi_xfer(SPI_DUMMY); (void)spi_xfer(SPI_DUMMY);
    end_cmd();

    /* Parse CSD (v2 only for simplicity) */
    if ((csd[0] >> 6) == 1) {
        uint32_t csize = ((uint32_t)(csd[7] & 0x3F) << 16) |
                         ((uint32_t)(csd[8]) << 8) |
                         (uint32_t)(csd[9]);
        *out_sectors = (csize + 1) * 1024;
        return true;
    }
    return false;
}
