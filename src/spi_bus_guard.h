#ifndef SPI_BUS_GUARD_H
#define SPI_BUS_GUARD_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Shared SPI3 bus guard for W25Q32 flash (RA9 CS) and SD card (RC11 CS).
 * Goal: never talk to both at the same time, and ensure the "other" CS is HIGH.
 *
 * This is a *cooperative* guard intended for your protothreads/main loop.
 * It does not spin forever: TryAcquire returns immediately.
 */

typedef enum
{
    SPI_DEV_NONE  = 0,
    SPI_DEV_FLASH = 1,
    SPI_DEV_SD    = 2
} SPI_DEV;

void SPIBus_Init(void);

/* Non-blocking */
bool SPIBus_TryAcquire(SPI_DEV dev);
void SPIBus_Release(SPI_DEV dev);

/* Convenience helpers (non-blocking) */
static inline bool SPIBus_AcquireSD(void)    { return SPIBus_TryAcquire(SPI_DEV_SD); }
static inline bool SPIBus_AcquireFlash(void) { return SPIBus_TryAcquire(SPI_DEV_FLASH); }

/* Optional: request "slow" SPI for SD init, then "fast" for transfers */
void SPIBus_SetSpeed_SDInit(void);
void SPIBus_SetSpeed_SDFast(void);
void SPIBus_SetSpeed_FlashFast(void);

#endif /* SPI_BUS_GUARD_H */
