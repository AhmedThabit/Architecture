/*******************************************************************************
  SD Card Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sd.c

  Summary:
    SD card application implementation for Architecture project

  Description:
    This file contains the SD card application logic.
    Adapted from reference project (pic32mm_usb_curiosity).
    
    Functionality:
    1. Mount SD card
    2. Unmount and remount (test)
    3. Open source file (FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG)
    4. Create directory (Dir1)
    5. Copy file to Dir1/FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG
    6. Turn on LED when complete
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app_sd.h"
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/* SD card application data */
static APP_SD_DATA appSDData;

/* Read/write buffer for file operations */
static uint8_t BUFFER_ATTRIBUTES readWriteBuffer[APP_SD_DATA_LEN];

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_SD_Initialize(void)

  Remarks:
    See prototype in app_sd.h.
 */

void APP_SD_Initialize(void)
{
    /* Initialize state machine */
    appSDData.state = APP_SD_WAIT_START;
    appSDData.fileHandle = SYS_FS_HANDLE_INVALID;
    appSDData.fileHandle1 = SYS_FS_HANDLE_INVALID;
    appSDData.nBytesRead = 0;
    
    /* Set to true for automatic start, false to wait for APP_SD_Start() call */
    appSDData.autoStart = true;
}


/*******************************************************************************
  Function:
    void APP_SD_Start(void)

  Remarks:
    See prototype in app_sd.h.
 */

void APP_SD_Start(void)
{
    if (appSDData.state == APP_SD_WAIT_START)
    {
        appSDData.state = APP_SD_MOUNT_DISK;
    }
}


/*******************************************************************************
  Function:
    APP_SD_STATES APP_SD_GetState(void)

  Remarks:
    See prototype in app_sd.h.
 */

APP_SD_STATES APP_SD_GetState(void)
{
    return appSDData.state;
}


/******************************************************************************
  Function:
    void APP_SD_Tasks(void)

  Remarks:
    See prototype in app_sd.h.
 */

void APP_SD_Tasks(void)
{
    /* Execute SD card state machine */
    switch (appSDData.state)
    {
        case APP_SD_WAIT_START:
        {
            /* Wait for start trigger or auto-start */
            if (appSDData.autoStart)
            {
                appSDData.state = APP_SD_MOUNT_DISK;
            }
            break;
        }
        
        case APP_SD_MOUNT_DISK:
        {
            /* Attempt to mount the SD card */
            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                /* Mount failed, try again */
                appSDData.state = APP_SD_MOUNT_DISK;
            }
            else
            {
                /* Mount successful, unmount for testing */
                appSDData.state = APP_SD_UNMOUNT_DISK;
            }
            break;
        }

        case APP_SD_UNMOUNT_DISK:
        {
            /* Attempt to unmount the disk */
            if (SYS_FS_Unmount(SDCARD_MOUNT_NAME) != SYS_FS_RES_SUCCESS)
            {
                /* Unmount failed, try again */
                appSDData.state = APP_SD_UNMOUNT_DISK;
            }
            else
            {
                /* Unmount successful, mount again */
                appSDData.state = APP_SD_MOUNT_DISK_AGAIN;
            }
            break;
        }

        case APP_SD_MOUNT_DISK_AGAIN:
        {
            /* Mount the disk again after testing unmount */
            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != SYS_FS_RES_SUCCESS)
            {
                /* Mount failed, try again */
                appSDData.state = APP_SD_MOUNT_DISK_AGAIN;
            }
            else
            {
                /* Mount successful, set current drive */
                appSDData.state = APP_SD_SET_CURRENT_DRIVE;
            }
            break;
        }

        case APP_SD_SET_CURRENT_DRIVE:
        {
            /* Set current drive to avoid using absolute paths */
            if (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Error setting current drive */
                appSDData.state = APP_SD_ERROR;
            }
            else
            {
                /* Current drive set, open source file */
                appSDData.state = APP_SD_OPEN_FIRST_FILE;
            }
            break;
        }

        case APP_SD_OPEN_FIRST_FILE:
        {
            /* Open source file for reading */
            appSDData.fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, SYS_FS_FILE_OPEN_READ);
            
            if (appSDData.fileHandle == SYS_FS_HANDLE_INVALID)
            {
                /* Could not open file */
                appSDData.state = APP_SD_ERROR;
            }
            else
            {
                /* File opened, create destination directory */
                appSDData.state = APP_SD_CREATE_DIRECTORY;
            }
            break;
        }

        case APP_SD_CREATE_DIRECTORY:
        {
            /* Create directory for destination file */
            if (SYS_FS_DirectoryMake(SDCARD_DIR_NAME) == SYS_FS_RES_FAILURE)
            {
                /* Directory creation failed (might already exist, continue anyway) */
                /* In real application, check if directory exists */
            }
            
            /* Proceed to open destination file */
            appSDData.state = APP_SD_OPEN_SECOND_FILE;
            break;
        }

        case APP_SD_OPEN_SECOND_FILE:
        {
            /* Open destination file for writing inside Dir1 */
            appSDData.fileHandle1 = SYS_FS_FileOpen(
                SDCARD_DIR_NAME "/" SDCARD_FILE_NAME,
                SYS_FS_FILE_OPEN_WRITE
            );
            
            if (appSDData.fileHandle1 == SYS_FS_HANDLE_INVALID)
            {
                /* Could not create destination file */
                SYS_FS_FileClose(appSDData.fileHandle);
                appSDData.state = APP_SD_ERROR;
            }
            else
            {
                /* Both files open, start copy operation */
                appSDData.state = APP_SD_READ_WRITE_TO_FILE;
            }
            break;
        }
        
        case APP_SD_READ_WRITE_TO_FILE:
        {
            /* Read from source file */
            appSDData.nBytesRead = SYS_FS_FileRead(
                appSDData.fileHandle,
                (void *)readWriteBuffer,
                APP_SD_DATA_LEN
            );
            
            if (appSDData.nBytesRead == -1)
            {
                /* Read error */
                SYS_FS_FileClose(appSDData.fileHandle);
                SYS_FS_FileClose(appSDData.fileHandle1);
                appSDData.state = APP_SD_ERROR;
            }
            else
            {
                /* Write to destination file */
                if (SYS_FS_FileWrite(appSDData.fileHandle1, (const void *)readWriteBuffer, appSDData.nBytesRead) == -1)
                {
                    /* Write error */
                    SYS_FS_FileClose(appSDData.fileHandle);
                    SYS_FS_FileClose(appSDData.fileHandle1);
                    appSDData.state = APP_SD_ERROR;
                }
                else if (SYS_FS_FileEOF(appSDData.fileHandle) == 1)
                {
                    /* End of file reached, copy complete */
                    appSDData.state = APP_SD_CLOSE_FILE;
                }
                /* else: Continue reading and writing */
            }
            break;
        }

        case APP_SD_CLOSE_FILE:
        {
            /* Close both files */
            SYS_FS_FileClose(appSDData.fileHandle);
            SYS_FS_FileClose(appSDData.fileHandle1);

            /* Operation successful, go to idle state */
            appSDData.state = APP_SD_IDLE;
            break;
        }

        case APP_SD_IDLE:
        {
            /* SD card operation completed successfully */
            /* Turn on LED to indicate success */
            LED_On();
            
            /* Stay in idle state */
            break;
        }

        case APP_SD_ERROR:
        {
            /* SD card operation failed */
            /* LED stays off to indicate error */
            
            /* Stay in error state */
            break;
        }

        default:
        {
            /* Invalid state, go to error */
            appSDData.state = APP_SD_ERROR;
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
