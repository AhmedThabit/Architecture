#include "flash_w25q32.h"

#include "peripheral/spi/spi_master/plib_spi3_master.h"
#include "peripheral/gpio/plib_gpio.h"

#include "spi_bus_guard.h"   // <-- your bus guard

// Use RA9 for the W25Q32 chip-select.
#define FLASH_CS_LOW()   GPIO_RA9_FL_SS3_Clear()
#define FLASH_CS_HIGH()  GPIO_RA9_FL_SS3_Set()

// SD CS (shared bus) - keep deselected while talking to Flash
#define SD_CS_HIGH()     GPIO_RC11_SD_SS3_Set()

#define CMD_WRITE_ENABLE   0x06
#define CMD_PAGE_PROGRAM   0x02
#define CMD_READ_DATA      0x03
#define CMD_SECTOR_ERASE   0x20
#define CMD_READ_STATUS    0x05

// ---- internal helpers (assume bus already acquired + CS handling done) ----

static void Flash_WriteEnable_NoLock(void)
{
    uint8_t cmd = CMD_WRITE_ENABLE;

    FLASH_CS_LOW();
    SPI3_Write(&cmd, 1);
    while (SPI3_IsBusy());
    FLASH_CS_HIGH();
}

static void Flash_WaitBusy_NoLock(void)
{
    uint8_t cmd = CMD_READ_STATUS;
    uint8_t status = 0;

    do
    {
        FLASH_CS_LOW();
        SPI3_Write(&cmd, 1);
        while (SPI3_IsBusy());

        SPI3_Read(&status, 1);
        while (SPI3_IsBusy());
        FLASH_CS_HIGH();

    } while (status & 0x01u);
}

static void Flash_PageProgram_NoLock(uint32_t addr, const uint8_t *buf, size_t len)
{
    Flash_WriteEnable_NoLock();

    uint8_t cmd[4] = {
        CMD_PAGE_PROGRAM,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)addr
    };

    FLASH_CS_LOW();
    SPI3_Write(cmd, 4);
    while (SPI3_IsBusy());

    SPI3_Write((void*)buf, len);
    while (SPI3_IsBusy());

    FLASH_CS_HIGH();

    Flash_WaitBusy_NoLock();
}

// ---- public API (acquire/release bus here; NEVER spin forever on bus) ----

void Flash_Init(void)
{
    SPI3_Initialize();

    // Make CS pins digital outputs and deselect both devices
    ANSELACLR = (1u << 9);
    ANSELCCLR = (1u << 11);

    GPIO_RA9_FL_SS3_OutputEnable();
    GPIO_RC11_SD_SS3_OutputEnable();

    FLASH_CS_HIGH();
    SD_CS_HIGH();
}

bool Flash_Read(uint32_t addr, uint8_t *buf, size_t len)
{
    if (buf == NULL || len == 0) return true;

    // Non-blocking: if SD owns the bus, return false (do NOT freeze)
    if (!SPIBus_TryAcquire(SPI_DEV_FLASH))
        return false;

    // Ensure other devices are deselected
    SD_CS_HIGH();
    FLASH_CS_HIGH();

    uint8_t cmd[4] = {
        CMD_READ_DATA,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)addr
    };

    FLASH_CS_LOW();
    SPI3_Write(cmd, 4);
    while (SPI3_IsBusy());

    SPI3_Read(buf, len);
    while (SPI3_IsBusy());

    FLASH_CS_HIGH();

    SPIBus_Release(SPI_DEV_FLASH);
    return true;
}

bool Flash_EraseSector(uint32_t addr)
{
    if (!SPIBus_TryAcquire(SPI_DEV_FLASH))
        return false;

    SD_CS_HIGH();
    FLASH_CS_HIGH();

    Flash_WriteEnable_NoLock();

    uint8_t cmd[4] = {
        CMD_SECTOR_ERASE,
        (uint8_t)(addr >> 16),
        (uint8_t)(addr >> 8),
        (uint8_t)addr
    };

    FLASH_CS_LOW();
    SPI3_Write(cmd, 4);
    while (SPI3_IsBusy());
    FLASH_CS_HIGH();

    Flash_WaitBusy_NoLock();

    SPIBus_Release(SPI_DEV_FLASH);
    return true;
}

bool Flash_Write(uint32_t addr, const uint8_t *buf, size_t len)
{
    if (buf == NULL || len == 0) return true;

    if (!SPIBus_TryAcquire(SPI_DEV_FLASH))
        return false;

    SD_CS_HIGH();
    FLASH_CS_HIGH();

    while (len)
    {
        size_t pageOff = addr & 0xFFu;
        size_t space   = 256u - pageOff;
        size_t chunk   = (len < space) ? len : space;

        Flash_PageProgram_NoLock(addr, buf, chunk);

        addr += (uint32_t)chunk;
        buf  += chunk;
        len  -= chunk;
    }

    SPIBus_Release(SPI_DEV_FLASH);
    return true;
}

uint32_t Flash_CRC32(const void *data, size_t len)
{
    uint32_t crc = 0xFFFFFFFFu;
    const uint8_t *p = (const uint8_t*)data;

    while (len--)
    {
        crc ^= *p++;
        for (int i = 0; i < 8; i++)
            crc = (crc & 1u) ? (crc >> 1) ^ 0xEDB88320u : (crc >> 1);
    }

    return crc ^ 0xFFFFFFFFu;
}
