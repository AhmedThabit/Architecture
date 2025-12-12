#ifndef SD_SPI_LOWLEVEL_H
#define SD_SPI_LOWLEVEL_H

#include <stdint.h>
#include <stdbool.h>

void SD_SPI_InitLowSpeed(void);
void SD_SPI_SetHighSpeed(void);

uint8_t SD_SPI_TxRx(uint8_t d);

void SD_Select(void);
void SD_Deselect(void);

#endif // SD_SPI_LOWLEVEL_H
