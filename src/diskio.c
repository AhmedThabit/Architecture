#include "ff.h"
#include "diskio.h"
#include "sd_spi_lowlevel.h"

static BYTE CardType;
static DSTATUS Stat = STA_NOINIT;

// Send a command packet to SD card
static BYTE sd_send_cmd(BYTE cmd, DWORD arg)
{
    BYTE buf[6];
    BYTE crc = 0x01;

    if (cmd == 0)  crc = 0x95;  // CMD0
    if (cmd == 8)  crc = 0x87;  // CMD8

    buf[0] = 0x40 | cmd;
    buf[1] = (BYTE)(arg >> 24);
    buf[2] = (BYTE)(arg >> 16);
    buf[3] = (BYTE)(arg >> 8);
    buf[4] = (BYTE)(arg);
    buf[5] = crc;

    SD_Deselect();
    SD_SPI_TxRx(0xFF);
    SD_Select();

    for (int i = 0; i < 6; i++)
        SD_SPI_TxRx(buf[i]);

    BYTE resp, n = 10;
    do {
        resp = SD_SPI_TxRx(0xFF);
    } while ((resp & 0x80) && --n);

    return resp;
}

// Receive a data block (512 bytes)
static int sd_rcvr_block(BYTE *buff, UINT btr)
{
    BYTE token;
    UINT timeout = 20000;
    do {
        token = SD_SPI_TxRx(0xFF);
    } while ((token == 0xFF) && --timeout);

    if (token != 0xFE) return 0;

    for (UINT i = 0; i < btr; i++)
        buff[i] = SD_SPI_TxRx(0xFF);

    SD_SPI_TxRx(0xFF); // discard CRC
    SD_SPI_TxRx(0xFF);

    return 1;
}

// Send a data block (512 bytes)
static int sd_xmit_block(const BYTE *buff, BYTE token)
{
    if (token != 0xFD) SD_SPI_TxRx(token); // 0xFD = Stop tran, else data token

    if (token == 0xFC) {
        for (int i = 0; i < 512; i++)
            SD_SPI_TxRx(buff[i]);

        SD_SPI_TxRx(0xFF);    // dummy CRC
        SD_SPI_TxRx(0xFF);

        BYTE resp = SD_SPI_TxRx(0xFF);
        if ((resp & 0x1F) != 0x05) // data accepted?
            return 0;
    }

    return 1;
}

/* Public FatFs interface */

// Initialize disk
DSTATUS disk_initialize(BYTE pdrv)
{
    BYTE n, ty, ocr[4];

    if (pdrv != 0) return STA_NOINIT;

    SD_SPI_InitLowSpeed();

    SD_Deselect();
    for (n = 0; n < 10; n++) SD_SPI_TxRx(0xFF);

    ty = 0;
    if (sd_send_cmd(0, 0) == 1) {            // Enter Idle state
        if (sd_send_cmd(8, 0x1AA) == 1) {   // SDv2?
            for (n = 0; n < 4; n++) ocr[n] = SD_SPI_TxRx(0xFF);

            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
                // Wait for leaving idle (ACMD41 with HCS)
                while (sd_send_cmd(55, 0) <= 1 && sd_send_cmd(41, 1UL << 30))
                    ;

                if (sd_send_cmd(58, 0) == 0) {
                    for (n = 0; n < 4; n++) ocr[n] = SD_SPI_TxRx(0xFF);
                    ty = (ocr[0] & 0x40) ? 6 : 2; // SDHC or SDSC
                }
            }
        }
        else {
            // SDv1 or MMCv3
            BYTE cmd;
            if (sd_send_cmd(55, 0) <= 1 && sd_send_cmd(41, 0) <= 1) {
                ty = 2;       // SDSC
                cmd = 41;
            } else {
                ty = 1;       // MMC
                cmd = 1;
            }
            while (sd_send_cmd(cmd, 0)) ;
            if (sd_send_cmd(16, 512) != 0)  // Set R/W block length
                ty = 0;
        }
    }

    CardType = ty;
    SD_Deselect();

    if (ty) {
        Stat &= ~STA_NOINIT;
        SD_SPI_SetHighSpeed();
    } else {
        Stat = STA_NOINIT;
    }

    return Stat;
}

DSTATUS disk_status(BYTE pdrv)
{
    if (pdrv != 0) return STA_NOINIT;
    return Stat;
}

DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT)  return RES_NOTRDY;

    if (!(CardType & 4)) sector *= 512; // SDSC: byte address

    if (count == 1) {
        if (sd_send_cmd(17, sector) == 0) {
            if (sd_rcvr_block(buff, 512)) {
                SD_Deselect();
                return RES_OK;
            }
        }
    } else {
        if (sd_send_cmd(18, sector) == 0) {
            do {
                if (!sd_rcvr_block(buff, 512)) break;
                buff += 512;
            } while (--count);
            sd_send_cmd(12, 0);
        }
    }

    SD_Deselect();
    return RES_ERROR;
}

DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT)  return RES_NOTRDY;

    if (!(CardType & 4)) sector *= 512;

    if (count == 1) {
        if (sd_send_cmd(24, sector) == 0) {
            if (sd_xmit_block(buff, 0xFC)) {
                SD_Deselect();
                return RES_OK;
            }
        }
    } else {
        if (sd_send_cmd(25, sector) == 0) {
            do {
                if (!sd_xmit_block(buff, 0xFC)) break;
                buff += 512;
            } while (--count);
            sd_xmit_block(0, 0xFD);
        }
    }

    SD_Deselect();
    return RES_ERROR;
}

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff)
{
    if (pdrv != 0) return RES_PARERR;
    if (Stat & STA_NOINIT)  return RES_NOTRDY;

    DRESULT res = RES_ERROR;

    switch (cmd) {
        case CTRL_SYNC:
            SD_Select();
            SD_Deselect();
            res = RES_OK;
            break;

        case GET_SECTOR_SIZE:
            *(WORD*)buff = 512;
            res = RES_OK;
            break;

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            res = RES_OK;
            break;

        case GET_SECTOR_COUNT:
            // You can refine this via CMD9 if needed
            *(DWORD*)buff = 0; // unknown
            res = RES_OK;
            break;

        default:
            res = RES_PARERR;
            break;
    }

    return res;
}
