#ifndef SD_RAW_H
#define SD_RAW_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Call once */
bool SDRAW_Init(void);

/* Sector I/O (512 bytes per sector) */
bool SDRAW_ReadSectors(uint32_t lba, uint8_t* buf, uint32_t count);
bool SDRAW_WriteSectors(uint32_t lba, const uint8_t* buf, uint32_t count);

/* Info */
bool SDRAW_GetSectorCount(uint32_t* out_sectors);

#endif
