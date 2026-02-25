// phonebook_flash.c
#include <string.h>
#include "drivers/flash/flash_w25q32.h"
#include "phonebook_flash.h"

//#define PB_BASE_ADDR  0x00000000u   // first sector of flash
#define PB_BASE_ADDR  0x00010000u   // next sector (example)


typedef struct
{
    char    numbers[PB_SLOT_COUNT][PB_MAX_LEN];  // 16 x 24 = 384 bytes
    uint8_t defaultSlot;                         // 0..15, or 0xFF = none
    uint8_t reserved[3];                         // padding / future use
    uint32_t crc;                                // CRC over everything except crc itself
} PB_Image;

static PB_Image g_pb;

// ---- internal helpers ----

static bool is_all_ff(const uint8_t *p, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        if (p[i] != 0xFF) return false;
    }
    return true;
}

static void pb_flush_to_flash(void)
{
    // compute CRC over all but last 4 bytes
    g_pb.crc = 0;
    uint32_t crc = Flash_CRC32(&g_pb, sizeof(PB_Image) - sizeof(uint32_t));
    g_pb.crc = crc;

    Flash_EraseSector(PB_BASE_ADDR);
    Flash_Write(PB_BASE_ADDR, (const uint8_t*)&g_pb, sizeof(PB_Image));
}

void PhonebookFlash_Init(void)
{
    // assume Flash_Init() already called somewhere in system init

    Flash_Read(PB_BASE_ADDR, (uint8_t*)&g_pb, sizeof(PB_Image));

    if (is_all_ff((uint8_t*)&g_pb, sizeof(PB_Image))) {
        // virgin flash -> create empty phonebook
        memset(&g_pb, 0, sizeof(PB_Image));
        g_pb.defaultSlot = 0xFF;
        pb_flush_to_flash();
        return;
    }

    // validate CRC
    uint32_t stored = g_pb.crc;
    g_pb.crc = 0;
    uint32_t calc = Flash_CRC32(&g_pb, sizeof(PB_Image) - sizeof(uint32_t));

    if (stored != calc) {
        // corrupted -> reset
        memset(&g_pb, 0, sizeof(PB_Image));
        g_pb.defaultSlot = 0xFF;
        pb_flush_to_flash();
    } else {
        g_pb.crc = stored;
    }
}

bool PhonebookFlash_SetNumber(uint8_t slot, const char *num)
{
    if (slot >= PB_SLOT_COUNT || num == NULL)
        return false;

    // clear and copy (truncate if needed)
    memset(g_pb.numbers[slot], 0, PB_MAX_LEN);
    size_t len = strlen(num);
    if (len >= PB_MAX_LEN) len = PB_MAX_LEN - 1;
    memcpy(g_pb.numbers[slot], num, len);

    pb_flush_to_flash();
    return true;
}

bool PhonebookFlash_GetNumber(uint8_t slot, char *out, size_t outSize)
{
    if (slot >= PB_SLOT_COUNT || out == NULL || outSize == 0)
        return false;

    size_t len = strnlen(g_pb.numbers[slot], PB_MAX_LEN);
    if (len + 1 > outSize) len = outSize - 1;

    memcpy(out, g_pb.numbers[slot], len);
    out[len] = '\0';
    return true;
}

bool PhonebookFlash_SetDefault(uint8_t slot)
{
    if (slot >= PB_SLOT_COUNT)
        return false;

    g_pb.defaultSlot = slot;
    pb_flush_to_flash();
    return true;
}

uint8_t PhonebookFlash_GetDefault(void)
{
    return g_pb.defaultSlot;
}

bool PhonebookFlash_Clear(void)
{
    memset(&g_pb, 0, sizeof(PB_Image));
    g_pb.defaultSlot = 0xFF;
    pb_flush_to_flash();
    return true;
}
