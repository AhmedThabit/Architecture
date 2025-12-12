#ifndef DISKIO_H
#define DISKIO_H

#include "integer.h"

// Status of Disk Functions
typedef BYTE   DSTATUS;

// Results of Disk Functions
typedef enum {
    RES_OK = 0,   // 0: Successful
    RES_ERROR,    // 1: R/W Error
    RES_WRPRT,    // 2: Write Protected
    RES_NOTRDY,   // 3: Not Ready
    RES_PARERR    // 4: Invalid Parameter
} DRESULT;

#define STA_NOINIT   0x01    // Drive not initialized
#define STA_NODISK   0x02    // No medium in the drive
#define STA_PROTECT  0x04    // Write protected

// Function prototypes (single drive -> pdrv=0)
DSTATUS disk_initialize(BYTE pdrv);
DSTATUS disk_status(BYTE pdrv);
DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff);

// Disk control command codes (subset)
#define CTRL_SYNC          0
#define GET_SECTOR_COUNT   1
#define GET_SECTOR_SIZE    2
#define GET_BLOCK_SIZE     3

#endif // DISKIO_H
