/*******************************************************************************
  SD Card Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app_sd.h

  Summary:
    SD card application header for Architecture project

  Description:
    This header file provides prototypes and definitions for SD card operations.
    Adapted from reference project (pic32mm_usb_curiosity) for integration with
    Architecture project that uses Protothreads.
*******************************************************************************/

#ifndef APP_SD_H
#define APP_SD_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "config/default/configuration.h"
#include "config/default/system/fs/sys_fs.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: SD Card Configuration
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    "/mnt/mydrive"
#define SDCARD_DEV_NAME      "/dev/mmcblka1"
#define SDCARD_FILE_NAME     "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG"
#define SDCARD_DIR_NAME      "Dir1"

#define APP_SD_DATA_LEN      512

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* SD Card Application States

  Summary:
    SD card application states enumeration

  Description:
    This enumeration defines the valid SD card application states.
*/

typedef enum
{
    /* Wait for switch press or auto-start */
    APP_SD_WAIT_START = 0,
            
    /* Mount the SD card */
    APP_SD_MOUNT_DISK,

    /* Unmount the disk */
    APP_SD_UNMOUNT_DISK,

    /* Mount the disk again */
    APP_SD_MOUNT_DISK_AGAIN,

    /* Set the current drive */
    APP_SD_SET_CURRENT_DRIVE,

    /* Open the source file to read */
    APP_SD_OPEN_FIRST_FILE,

    /* Create directory */
    APP_SD_CREATE_DIRECTORY,

    /* Open the destination file to write */
    APP_SD_OPEN_SECOND_FILE,

    /* Read from source and write to destination */
    APP_SD_READ_WRITE_TO_FILE,

    /* Close files */
    APP_SD_CLOSE_FILE,

    /* Idle state - operation complete */
    APP_SD_IDLE,

    /* Error state */
    APP_SD_ERROR

} APP_SD_STATES;


// *****************************************************************************
/* SD Card Application Data

  Summary:
    Holds SD card application data

  Description:
    This structure holds the SD card application's data.
*/

typedef struct
{
    /* SYS_FS File handle for source file */
    SYS_FS_HANDLE       fileHandle;

    /* SYS_FS File handle for destination file */
    SYS_FS_HANDLE       fileHandle1;

    /* Application's current state */
    APP_SD_STATES       state;

    /* Number of bytes read */
    int32_t             nBytesRead;
    
    /* Auto-start flag (true = start automatically, false = wait for trigger) */
    bool                autoStart;

} APP_SD_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_SD_Initialize(void)

  Summary:
     SD card application initialization routine.

  Description:
    Initializes the SD card application state machine.

  Precondition:
    SYS_Initialize must have been called.

  Parameters:
    None.

  Returns:
    None.
*/
void APP_SD_Initialize(void);


/*******************************************************************************
  Function:
    void APP_SD_Tasks(void)

  Summary:
    SD card application tasks function

  Description:
    This routine implements the SD card application's state machine.
    Call this function periodically from main loop or Protothread.

  Precondition:
    APP_SD_Initialize must have been called.

  Parameters:
    None.

  Returns:
    None.
*/
void APP_SD_Tasks(void);


/*******************************************************************************
  Function:
    void APP_SD_Start(void)

  Summary:
    Trigger SD card operation to start

  Description:
    Call this function to trigger the SD card copy operation.
    Useful when autoStart is false.

  Parameters:
    None.

  Returns:
    None.
*/
void APP_SD_Start(void);


/*******************************************************************************
  Function:
    APP_SD_STATES APP_SD_GetState(void)

  Summary:
    Get current SD card state

  Description:
    Returns the current state of the SD card state machine.

  Parameters:
    None.

  Returns:
    Current APP_SD_STATES value
*/
APP_SD_STATES APP_SD_GetState(void);


#ifdef __cplusplus
}
#endif

#endif /* APP_SD_H */

/*******************************************************************************
 End of File
 */
