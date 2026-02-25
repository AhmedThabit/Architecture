#ifndef SD_FATFS_GUARD_H
#define SD_FATFS_GUARD_H

#include "ff.h"
#include "hal/spi_guard/spi_bus_guard.h"

static inline FRESULT SD_f_mount(FATFS* fs, const TCHAR* path, BYTE opt)
{
    FRESULT r;
    while (!SPIBus_AcquireSD()) {}
    r = f_mount(fs, path, opt);
    SPIBus_Release(SPI_DEV_SD);
    return r;
}

static inline FRESULT SD_f_getfree(const TCHAR* path, DWORD* nclust, FATFS** fatfs)
{
    FRESULT r;
    while (!SPIBus_AcquireSD()) {}
    r = f_getfree(path, nclust, fatfs);
    SPIBus_Release(SPI_DEV_SD);
    return r;
}

static inline FRESULT SD_f_opendir(DIR* dp, const TCHAR* path)
{
    FRESULT r;
    while (!SPIBus_AcquireSD()) {}
    r = f_opendir(dp, path);
    SPIBus_Release(SPI_DEV_SD);
    return r;
}

static inline FRESULT SD_f_readdir(DIR* dp, FILINFO* fno)
{
    FRESULT r;
    while (!SPIBus_AcquireSD()) {}
    r = f_readdir(dp, fno);
    SPIBus_Release(SPI_DEV_SD);
    return r;
}

static inline FRESULT SD_f_closedir(DIR* dp)
{
    FRESULT r;
    while (!SPIBus_AcquireSD()) {}
    r = f_closedir(dp);
    SPIBus_Release(SPI_DEV_SD);
    return r;
}

#endif
