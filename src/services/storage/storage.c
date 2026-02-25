/*
 * storage.c
 *
 * Implementation of a simple storage layer for phone numbers and
 * audio recordings on a W25Q32 serial flash.  The module defines a
 * header structure that is stored in the first 4 kB sector of the
 * flash and contains arrays for phone numbers, recording offsets,
 * lengths and a pointer to the next free byte.  A CRC32 covers
 * all fields except itself to detect corruption.  Audio data is
 * appended sequentially after the header.  See storage.h for API.
 */

#include "storage.h"
#include "drivers/flash/flash_w25q32.h"
#include <string.h>

/* Starting address of the header in flash.  Sector 0 (0x0000)
 * is reserved for the header.  Audio data starts at the next
 * sector boundary (4 kB offset).  If you change this value,
 * update AUDIO_START_ADDR accordingly. */
#define FLASH_HEADER_ADDR 0x00000000UL

/* Offset where audio data begins.  This must be aligned to a
 * 4 kB sector boundary because erase operations occur per sector.
 * We choose 0x1000 (4096 B) to skip the header sector. */
#define AUDIO_START_ADDR  0x00001000UL

/* Total size of the W25Q32 in bytes (32 Mbit = 4 Mbyte). */
#define FLASH_TOTAL_SIZE  (4UL * 1024UL * 1024UL)

/* Flash erase unit size (4 kB sectors). */
#define FLASH_SECTOR_SIZE 4096UL

/* Internal header structure stored at FLASH_HEADER_ADDR.  It
 * includes phone numbers, recording offsets/lengths and a CRC. */
typedef struct {
    char numbers[STORAGE_MAX_ENTRIES][STORAGE_MAX_NUMBER_LEN];
    uint32_t offsets[STORAGE_MAX_ENTRIES];
    uint32_t lengths[STORAGE_MAX_ENTRIES];
    uint32_t nextFreeOffset;
    uint32_t crc; /* CRC over all preceding fields */
} FlashHeader;

/* Static header instance used at run time. */
static FlashHeader gHeader;

/* Forward declaration */
static void header_update_crc(FlashHeader *h);
static bool header_valid(const FlashHeader *h);
static bool write_header_to_flash(const FlashHeader *h);

/* Compute CRC32 over the header except the crc field itself. */
static void header_update_crc(FlashHeader *h)
{
    /* Compute CRC over the header up to (but excluding) the crc field. */
    uint32_t computed = Flash_CRC32(h, offsetof(FlashHeader, crc));
    h->crc = computed;
}

/* Verify that the crc field matches the computed CRC. */
static bool header_valid(const FlashHeader *h)
{
    uint32_t computed = Flash_CRC32(h, offsetof(FlashHeader, crc));
    return (computed == h->crc);
}

/* Erase the header sector and write the header to flash. */
static bool write_header_to_flash(const FlashHeader *h)
{
    /* Erase the first sector */
    Flash_EraseSector(FLASH_HEADER_ADDR);
    /* Write the header.  Use sizeof to include all fields. */
    Flash_Write(FLASH_HEADER_ADDR, (const uint8_t *)h, sizeof(FlashHeader));
    /* It would be prudent to verify the write by reading back and
     * checking the CRC, but we rely on the underlying Flash_Write to
     * handle errors.  In production code you may want to add a
     * verification step. */
    return true;
}

void Storage_Init(void)
{
    /* Initialise SPI and chip select pins for the flash. */
    Flash_Init();
    /* Read the existing header from flash. */
    Flash_Read(FLASH_HEADER_ADDR, (uint8_t *)&gHeader, sizeof(FlashHeader));
    /* Validate CRC.  If invalid, initialise a new empty header. */
    if (!header_valid(&gHeader)) {
        /* Clear all phone numbers and offsets. */
        memset(&gHeader, 0, sizeof(gHeader));
        /* nextFreeOffset starts at 0 relative to AUDIO_START_ADDR */
        gHeader.nextFreeOffset = 0;
        /* Compute CRC and write to flash. */
        header_update_crc(&gHeader);
        write_header_to_flash(&gHeader);
    }
}

bool Storage_SavePhoneNumber(int slot, const char *number)
{
    if (slot < 0 || slot >= STORAGE_MAX_ENTRIES || number == NULL) {
        return false;
    }
    /* Copy and truncate number into the header */
    memset(gHeader.numbers[slot], 0, STORAGE_MAX_NUMBER_LEN);
    strncpy(gHeader.numbers[slot], number, STORAGE_MAX_NUMBER_LEN - 1);
    /* Update CRC and write header */
    header_update_crc(&gHeader);
    return write_header_to_flash(&gHeader);
}

bool Storage_GetPhoneNumber(int slot, char *outNumber)
{
    if (slot < 0 || slot >= STORAGE_MAX_ENTRIES || outNumber == NULL) {
        return false;
    }
    /* Check if the stored number is non‑empty */
    if (gHeader.numbers[slot][0] == '\0') {
        return false;
    }
    strncpy(outNumber, gHeader.numbers[slot], STORAGE_MAX_NUMBER_LEN);
    return true;
}

/* Helper to erase all sectors covering a region */
static void erase_sectors_covering(uint32_t startAddr, size_t len)
{
    uint32_t endAddr = startAddr + len;
    /* Align start to sector boundary */
    uint32_t sectorAddr = startAddr & ~(FLASH_SECTOR_SIZE - 1);
    while (sectorAddr < endAddr) {
        Flash_EraseSector(sectorAddr);
        sectorAddr += FLASH_SECTOR_SIZE;
    }
}

bool Storage_SaveRecording(int slot, const uint8_t *data, size_t len)
{
    if (slot < 0 || slot >= STORAGE_MAX_ENTRIES || data == NULL || len == 0) {
        return false;
    }
    /* Ensure there is no existing recording in this slot.  This simple
     * implementation does not support overwriting individual
     * recordings; instead you could clear the header and start
     * afresh. */
    if (gHeader.lengths[slot] != 0) {
        return false; /* Already used */
    }
    /* Compute absolute flash address where this recording will be stored */
    uint32_t absOffset = AUDIO_START_ADDR + gHeader.nextFreeOffset;
    /* Check for overflow of flash memory */
    if (absOffset + len > FLASH_TOTAL_SIZE) {
        return false;
    }
    /* Erase sectors needed for this recording */
    erase_sectors_covering(absOffset, len);
    /* Write the audio data */
    Flash_Write(absOffset, data, len);
    /* Update header entry */
    gHeader.offsets[slot] = gHeader.nextFreeOffset;
    gHeader.lengths[slot] = (uint32_t)len;
    /* Advance next free offset */
    gHeader.nextFreeOffset += (uint32_t)len;
    /* Update CRC and write header */
    header_update_crc(&gHeader);
    return write_header_to_flash(&gHeader);
}

bool Storage_ReadRecording(int slot, uint8_t *outData, size_t *outLen)
{
    if (slot < 0 || slot >= STORAGE_MAX_ENTRIES || outData == NULL || outLen == NULL) {
        return false;
    }
    uint32_t len = gHeader.lengths[slot];
    if (len == 0) {
        return false; /* No recording stored */
    }
    uint32_t absOffset = AUDIO_START_ADDR + gHeader.offsets[slot];
    /* Read data from flash */
    Flash_Read(absOffset, outData, len);
    *outLen = len;
    return true;
}