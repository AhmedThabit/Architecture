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
#include "configuration.h"
#include "system/fs/sys_fs.h"
#include "bsp.h"
#include "config/default/user.h"

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




///*******************************************************************************
//  MPLAB Harmony Application Header File
//
//  Company:
//    Microchip Technology Inc.
//
//  File Name:
//    app.h
//
//  Summary:
//    This header file provides prototypes and definitions for the application.
//
//  Description:
//    This header file provides function prototypes and data type definitions for
//    the application.  Some of these are required by the system (such as the
//    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
//    internally by the application (such as the "APP_STATES" definition).  Both
//    are defined here for convenience.
//*******************************************************************************/
//
////DOM-IGNORE-BEGIN
///*******************************************************************************
//* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
//*
//* Subject to your compliance with these terms, you may use Microchip software
//* and any derivatives exclusively with Microchip products. It is your
//* responsibility to comply with third party license terms applicable to your
//* use of third party software (including open source software) that may
//* accompany Microchip software.
//*
//* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
//* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
//* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
//* PARTICULAR PURPOSE.
//*
//* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
//* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
//* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
//* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
//* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
//* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
//* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//*******************************************************************************/
////DOM-IGNORE-END
//
//#ifndef APP_H
//#define APP_H
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Included Files
//// *****************************************************************************
//// *****************************************************************************
//
//#include <stdint.h>
//#include <stdbool.h>
//#include <stddef.h>
//#include <stdlib.h>
//#include "configuration.h"
//#include "system/fs/sys_fs.h"
//
//// DOM-IGNORE-BEGIN
//#ifdef __cplusplus  // Provide C++ Compatibility
//
//extern "C" {
//
//#endif
//// DOM-IGNORE-END
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Type Definitions
//// *****************************************************************************
//// *****************************************************************************
//
//// *****************************************************************************
///* Application states
//
//  Summary:
//    Application states enumeration
//
//  Description:
//    This enumeration defines the valid application states.  These states
//    determine the behavior of the application at various times.
//*/
//
//typedef enum
//{
//    /* Application's state machine's initial state. */
//    /* The app mounts the disk */
//    APP_WAIT_SWITCH_PRESS = 0,
//            
//    APP_MOUNT_DISK,
//
//    /* The app unmounts the disk */
//    APP_UNMOUNT_DISK,
//
//    /* The app mounts the disk again */
//    APP_MOUNT_DISK_AGAIN,
//
//        /* Set the current drive */
//    APP_SET_CURRENT_DRIVE,
//
//    /* The app opens the file to read */
//    APP_OPEN_FIRST_FILE,
//
//        /* Create directory */
//    APP_CREATE_DIRECTORY,
//
//        /* The app opens the file to write */
//    APP_OPEN_SECOND_FILE,
//
//    /* The app reads from a file and writes to another file */
//    APP_READ_WRITE_TO_FILE,
//
//    /* The app closes the file*/
//    APP_CLOSE_FILE,
//
//    /* The app closes the file and idles */
//    APP_IDLE,
//
//    /* An app error has occurred */
//    APP_ERROR
//
//} APP_STATES;
//
//
//// *****************************************************************************
///* Application Data
//
//  Summary:
//    Holds application data
//
//  Description:
//    This structure holds the application's data.
//
//  Remarks:
//    Application strings and buffers are be defined outside this structure.
// */
//
//typedef struct
//{
//    /* SYS_FS File handle for 1st file */
//    SYS_FS_HANDLE               fileHandle;
//
//    /* SYS_FS File handle for 2nd file */
//    SYS_FS_HANDLE               fileHandle1;
//
//    /* Application's current state */
//    APP_STATES                  state;           
//
//    int32_t                     nBytesRead;
//} APP_DATA;
//
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Application Callback Routines
//// *****************************************************************************
//// *****************************************************************************
///* These routines are called by drivers when certain events occur.
//*/
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Application Initialization and State Machine Functions
//// *****************************************************************************
//// *****************************************************************************
//
///*******************************************************************************
//  Function:
//    void APP_Initialize ( void )
//
//  Summary:
//     MPLAB Harmony application initialization routine.
//
//  Description:
//    This function initializes the Harmony application.  It places the
//    application in its initial state and prepares it to run so that its
//    APP_Tasks function can be called.
//
//  Precondition:
//    All other system initialization routines should be called before calling
//    this routine (in "SYS_Initialize").
//
//  Parameters:
//    None.
//
//  Returns:
//    None.
//
//  Example:
//    <code>
//    APP_Initialize();
//    </code>
//
//  Remarks:
//    This routine must be called from the SYS_Initialize function.
//*/
//
//void APP_Initialize ( void );
//
//
///*******************************************************************************
//  Function:
//    void APP_Tasks ( void )
//
//  Summary:
//    MPLAB Harmony Demo application tasks function
//
//  Description:
//    This routine is the Harmony Demo application's tasks function.  It
//    defines the application's state machine and core logic.
//
//  Precondition:
//    The system and application initialization ("SYS_Initialize") should be
//    called before calling this.
//
//  Parameters:
//    None.
//
//  Returns:
//    None.
//
//  Example:
//    <code>
//    APP_Tasks();
//    </code>
//
//  Remarks:
//    This routine must be called from SYS_Tasks() routine.
// */
//
//void APP_Tasks( void );
//
//
//#endif /* APP_H */
//
////DOM-IGNORE-BEGIN
//#ifdef __cplusplus
//}
//#endif
////DOM-IGNORE-END
//
///*******************************************************************************
// End of File
// */
//
