/*******************************************************************************
  System Configuration Header - MERGED VERSION
  Combines: New project settings + SD Card File System settings
 *******************************************************************************/

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// Include your existing device headers
#include "user.h"
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// SD CARD FILE SYSTEM CONFIGURATION (From SD Project)
// ============================================================================

/* File System Service Configuration */
#define SYS_FS_MEDIA_NUMBER               (1U)
#define SYS_FS_VOLUME_NUMBER              (1U)
#define SYS_FS_AUTOMOUNT_ENABLE           false
#define SYS_FS_MAX_FILES                  (2U)        // Max 2 files open (copy operation)
#define SYS_FS_MAX_FILE_SYSTEM_TYPE       (1U)
#define SYS_FS_MEDIA_MAX_BLOCK_SIZE       (512)
#define SYS_FS_MEDIA_MANAGER_BUFFER_SIZE  (2048U)
#define SYS_FS_USE_LFN                    (1)         // Long filename support
#define SYS_FS_FILE_NAME_LEN              (255U)
#define SYS_FS_CWD_STRING_LEN             (1024)

/* FAT FS Configuration */
#define SYS_FS_FAT_VERSION                "v0.15"
#define SYS_FS_FAT_READONLY               false
#define SYS_FS_FAT_CODE_PAGE              437
#define SYS_FS_FAT_MAX_SS                 SYS_FS_MEDIA_MAX_BLOCK_SIZE

// ============================================================================
// SDSPI DRIVER CONFIGURATION (From SD Project)
// ============================================================================

/* SDSPI Driver Instance 0 Configuration */
#define DRV_SDSPI_INDEX_0                       0
#define DRV_SDSPI_CLIENTS_NUMBER_IDX0           1
#define DRV_SDSPI_QUEUE_SIZE_IDX0               4
#define DRV_SDSPI_CHIP_SELECT_PIN_IDX0          SYS_PORT_PIN_RC11   // Adjust to your board!
#define DRV_SDSPI_SPEED_HZ_IDX0                 5000000             // 5 MHz SPI
#define DRV_SDSPI_POLLING_INTERVAL_MS_IDX0      1000

/* SDSPI Driver Common Configuration */
#define DRV_SDSPI_INSTANCES_NUMBER              (1U)

// ============================================================================
// TIME SYSTEM SERVICE (Keep your existing or use SD project style)
// ============================================================================

/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                            (0)
#define SYS_TIME_MAX_TIMERS                         (5)
#define SYS_TIME_HW_COUNTER_WIDTH                   (32)
#define SYS_TIME_HW_COUNTER_PERIOD                  (4294967295U)
#define SYS_TIME_HW_COUNTER_HALF_PERIOD             (SYS_TIME_HW_COUNTER_PERIOD>>1)
#define SYS_TIME_CPU_CLOCK_FREQUENCY                (48000000)  // 24 MHz - adjust to your clock
#define SYS_TIME_COMPARE_UPDATE_EXECUTION_CYCLES    (460)

// ============================================================================
// YOUR EXISTING CONFIGURATIONS (Keep from new project)
// ============================================================================

// Add your existing configuration macros here
// e.g., UART configs, W5500 configs, etc.

#ifdef __cplusplus
}
#endif

#endif // CONFIGURATION_H