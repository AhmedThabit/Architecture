#include "sd_service.h"

#include "definitions.h"
#include "spi_bus_guard.h"

/*
 * This service mounts the SD card using Harmony SYS_FS (not raw Chan f_mount).
 * That matches the known-good reference project you provided.
 *
 * - Waits for DRV_SDSPI to be READY
 * - Acquires the shared SPI bus (deselects Flash)
 * - Calls SYS_FS_Mount()
 * - Marks sd_mounted when mounted
 *
 * IMPORTANT:
 *  1) Your MCC config defines the SD device name and mount name.
 *     In Harmony 3 these usually exist as SYS_FS_MEDIA_IDX0_DEVICE_NAME and
 *     SYS_FS_MEDIA_IDX0_MOUNT_NAME (or similar).
 *  2) If not found, we fall back to typical defaults used in the reference project:
 *        dev  = "/dev/mmcblka1"
 *        mnt  = "/mnt/mydrive"
 */

#ifndef SD_DEV_NAME
    #if defined(SYS_FS_MEDIA_IDX0_DEVICE_NAME)
        #define SD_DEV_NAME SYS_FS_MEDIA_IDX0_DEVICE_NAME
    #else
        #define SD_DEV_NAME "/dev/mmcblka1"
    #endif
#endif

#ifndef SD_MOUNT_NAME
    #if defined(SYS_FS_MEDIA_IDX0_MOUNT_NAME)
        #define SD_MOUNT_NAME SYS_FS_MEDIA_IDX0_MOUNT_NAME
    #else
        #define SD_MOUNT_NAME "/mnt/mydrive"
    #endif
#endif

static volatile bool g_sd_mounted = false;
static SYS_FS_HANDLE g_mount_handle = SYS_FS_HANDLE_INVALID;

/* simple retry pacing */
static uint32_t g_next_try_ms = 0;

void SD_Service_Init(void)
{
    g_sd_mounted = false;
    g_mount_handle = SYS_FS_HANDLE_INVALID;
    g_next_try_ms = 0;
}

bool SD_Service_IsMounted(void)
{
    return g_sd_mounted;
}

const char* SD_Service_MountPoint(void)
{
    return SD_MOUNT_NAME;
}

void SD_Service_Task(void)
{
    if (g_sd_mounted)
        return;

    /* Wait until Harmony driver objects are ready */
#if defined(sysObj) && defined(DRV_SDSPI_Status)
    if (DRV_SDSPI_Status(sysObj.drvSDSPI0) != SYS_STATUS_READY)
        return;
#else
    /* If sysObj isn't visible, you can expose drvSDSPI0 externally or remove this gate. */
#endif

    /* Pace retries (avoid hammering mount call) */
    extern volatile uint32_t msTicks;
    if ((int32_t)(msTicks - g_next_try_ms) < 0)
        return;

    g_next_try_ms = msTicks + 500; /* retry every 500ms until success */

    /* Acquire shared SPI bus */
    if (!SPIBus_AcquireSD())
        return;

    /* SD init needs slow clock at first */
    SPIBus_SetSpeed_SDInit();

    /* Try to mount. SYS_FS_Mount returns SYS_FS_HANDLE_INVALID on failure/busy. */
    g_mount_handle = SYS_FS_Mount(SD_DEV_NAME, SD_MOUNT_NAME, FAT, 0, NULL);

    if (g_mount_handle != SYS_FS_HANDLE_INVALID)
    {
        /* SD is mounted; bump speed for data */
        SPIBus_SetSpeed_SDFast();
        g_sd_mounted = true;
    }

    SPIBus_Release(SPI_DEV_SD);
}
