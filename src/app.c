/*******************************************************************************
  ULTRA-MINIMAL TEST - NO LED, JUST UART
 *******************************************************************************/

#include "bsp.h"
#include "config/default/user.h"
#include "app.h"
#include "definitions.h"

extern void UART3_WriteString33(const char *str);

static APP_SD_DATA appSDData;
static uint8_t readWriteBuffer[APP_SD_DATA_LEN];

void APP_SD_Initialize(void)
{
    UART3_WriteString33("\r\n*** MINIMAL TEST START ***\r\n");
    appSDData.state = APP_SD_WAIT_START;
}

void APP_SD_Start(void)
{
    // Empty
}

APP_SD_STATES APP_SD_GetState(void)
{
    return appSDData.state;
}

void APP_SD_Tasks(void)
{
    static uint32_t count = 0;
    count++;
    
    /* Print every 1000 calls */
    if (count % 1000 == 0)
    {
        UART3_WriteString33(".");
    }
    
    /* Print message every 10000 calls */
    if (count % 10000 == 0)
    {
        UART3_WriteString33(" OK\r\n");
    }
}

/*******************************************************************************
 End of File
 */

///*******************************************************************************
//  SD Card Application Source File - With UART3 Debug Output
//
//  Company:
//    Microchip Technology Inc.
//
//  File Name:
//    app.c
//
//  Summary:
//    SD card application with state machine debugging via UART3
//
//  Description:
//    This file contains the SD card application logic with comprehensive
//    UART3 debug output to track state machine progression.
//    
//    Functionality:
//    1. Mount SD card
//    2. Unmount and remount (test)
//    3. Open source file (FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG)
//    4. Create directory (Dir1)
//    5. Copy file to Dir1/FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG
//    6. Turn OFF LED when complete (LED ON during operation)
// *******************************************************************************/
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Included Files
//// *****************************************************************************
//// *****************************************************************************
//#include "bsp.h"
//#include "config/default/user.h"
//#include "app.h"
//#include "definitions.h"
//
//// Forward declaration of UART3_WriteString33 (defined in main.c)
//extern void UART3_WriteString33(const char *str);
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Global Data Definitions
//// *****************************************************************************
//// *****************************************************************************
//
///* SD card application data */
//static APP_SD_DATA appSDData;
//
///* Read/write buffer for file operations */
//static uint8_t readWriteBuffer[APP_SD_DATA_LEN];
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Helper Functions
//// *****************************************************************************
//// *****************************************************************************
//
///* Convert integer to string for debug output */
//static void int_to_str(int32_t num, char* str) {
//    if (num < 0) {
//        *str++ = '-';
//        num = -num;
//    }
//
//    if (num == 0) {
//        *str++ = '0';
//        *str = '\0';
//        return;
//    }
//
//    char temp[12];
//    int i = 0;
//    while (num > 0) {
//        temp[i++] = '0' + (num % 10);
//        num /= 10;
//    }
//
//    while (i > 0) {
//        *str++ = temp[--i];
//    }
//    *str = '\0';
//}
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Application Initialization and State Machine Functions
//// *****************************************************************************
//// *****************************************************************************
//
///*******************************************************************************
//  Function:
//    void APP_SD_Initialize(void)
//
//  Remarks:
//    See prototype in app.h.
// */
//
//void APP_SD_Initialize(void) {
//    UART3_WriteString33("\r\n=== SD CARD APP INIT ===\r\n");
//
//    /* Initialize state machine */
//    appSDData.state = APP_SD_WAIT_START;
//    appSDData.fileHandle = SYS_FS_HANDLE_INVALID;
//    appSDData.fileHandle1 = SYS_FS_HANDLE_INVALID;
//    appSDData.nBytesRead = 0;
//
//    /* Set to true for automatic start */
//    //appSDData.autoStart = true;
//    // AFTER: Wait for drivers to initialize
//appSDData.autoStart = false;
////startupDelayCounter = 0;
//// Waits 100 cycles before trying to mount
//// Check if SYS_TIME is ready
//if (SYS_TIME_DelayMS(1, NULL) == SYS_TIME_SUCCESS)
//{
//    UART3_WriteString33("SD: Drivers ready! Starting...\r\n");
//    appSDData.state = APP_SD_MOUNT_DISK;
//}
//
//    UART3_WriteString33("State: WAIT_START\r\n");
//    UART3_WriteString33("Auto-start: ENABLED\r\n");
//
//    /* Diagnostic: Check Timer2 status */
//    UART3_WriteString33("\n--- Timer2 Check ---\r\n");
//    if (T2CON & _T2CON_ON_MASK) {
//        UART3_WriteString33("Timer2: ON\r\n");
//    } else {
//        UART3_WriteString33("ERROR: Timer2 OFF!\r\n");
//    }
//
//    if (IEC0 & _IEC0_T2IE_MASK) {
//        UART3_WriteString33("Timer2 INT: ENABLED\r\n");
//    } else {
//        UART3_WriteString33("ERROR: Timer2 INT disabled!\r\n");
//    }
//
//    UART3_WriteString33("====================\r\n\n");
//}
//
///*******************************************************************************
//  Function:
//    void APP_SD_Start(void)
//
//  Remarks:
//    See prototype in app.h.
// */
//
//void APP_SD_Start(void) {
//    if (appSDData.state == APP_SD_WAIT_START) {
//        UART3_WriteString33("SD: Manual start triggered\r\n");
//        appSDData.state = APP_SD_MOUNT_DISK;
//    }
//}
//
///*******************************************************************************
//  Function:
//    APP_SD_STATES APP_SD_GetState(void)
//
//  Remarks:
//    See prototype in app.h.
// */
//
//APP_SD_STATES APP_SD_GetState(void) {
//    return appSDData.state;
//}
//
///******************************************************************************
//  Function:
//    void APP_SD_Tasks(void)
//
//  Remarks:
//    See prototype in app.h.
// */
//
//void APP_SD_Tasks(void) {
//    /* Execute SD card state machine */
//    switch (appSDData.state) {
//        case APP_SD_WAIT_START:
//        {
//            /* Wait for start trigger or auto-start */
//            if (appSDData.autoStart) {
//                UART3_WriteString33("SD: Auto-starting...\r\n");
//                appSDData.state = APP_SD_MOUNT_DISK;
//            }
//            break;
//        }
//
//        case APP_SD_MOUNT_DISK:
//        {
//            /* Turn ON LED to show operation in progress */
//            LED1_On();
//            UART3_WriteString33("SD: Auto-starting...\r\n");
//            UART3_WriteString33("SD: About to call SYS_FS_Mount...\r\n"); // ADD THIS
//
//            /* Attempt to mount the SD card */
//            SYS_FS_RESULT result = SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL);
//
//            UART3_WriteString33("SD: SYS_FS_Mount returned\r\n"); // ADD THIS
//
//            if (result != SYS_FS_RES_SUCCESS) {
//                UART3_WriteString33("SD: Mount FAILED, retrying...\r\n");
//                appSDData.state = APP_SD_MOUNT_DISK;
//            } else {
//                UART3_WriteString33("SD: Mount OK!\r\n");
//                appSDData.state = APP_SD_UNMOUNT_DISK;
//            }
//            break;
//        }
//
//        case APP_SD_UNMOUNT_DISK:
//        {
//            UART3_WriteString33("SD: Unmounting (test)...\r\n");
//
//            /* Attempt to unmount the disk */
//            if (SYS_FS_Unmount(SDCARD_MOUNT_NAME) != SYS_FS_RES_SUCCESS) {
//                /* Unmount failed, try again */
//                UART3_WriteString33("SD: Unmount FAILED, retrying...\r\n");
//                appSDData.state = APP_SD_UNMOUNT_DISK;
//            } else {
//                /* Unmount successful */
//                UART3_WriteString33("SD: Unmount OK\r\n");
//                appSDData.state = APP_SD_MOUNT_DISK_AGAIN;
//            }
//            break;
//        }
//
//        case APP_SD_MOUNT_DISK_AGAIN:
//        {
//            UART3_WriteString33("SD: Re-mounting...\r\n");
//
//            /* Mount the disk again after testing unmount */
//            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != SYS_FS_RES_SUCCESS) {
//                /* Mount failed, try again */
//                UART3_WriteString33("SD: Re-mount FAILED, retrying...\r\n");
//                appSDData.state = APP_SD_MOUNT_DISK_AGAIN;
//            } else {
//                /* Mount successful */
//                UART3_WriteString33("SD: Re-mount OK!\r\n");
//                appSDData.state = APP_SD_SET_CURRENT_DRIVE;
//            }
//            break;
//        }
//
//        case APP_SD_SET_CURRENT_DRIVE:
//        {
//            UART3_WriteString33("SD: Setting current drive...\r\n");
//
//            /* Set current drive to avoid using absolute paths */
//            if (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE) {
//                /* Error setting current drive */
//                UART3_WriteString33("SD: Set drive FAILED!\r\n");
//                appSDData.state = APP_SD_ERROR;
//            } else {
//                /* Current drive set */
//                UART3_WriteString33("SD: Current drive set OK\r\n");
//                appSDData.state = APP_SD_OPEN_FIRST_FILE;
//            }
//            break;
//        }
//
//        case APP_SD_OPEN_FIRST_FILE:
//        {
//            UART3_WriteString33("SD: Opening source file: ");
//            UART3_WriteString33(SDCARD_FILE_NAME);
//            UART3_WriteString33("\r\n");
//
//            /* Open source file for reading */
//            appSDData.fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, SYS_FS_FILE_OPEN_READ);
//
//            if (appSDData.fileHandle == SYS_FS_HANDLE_INVALID) {
//                /* Could not open file */
//                UART3_WriteString33("SD: ERROR - Source file NOT FOUND!\r\n");
//                UART3_WriteString33("SD: Check SD card has file in root directory\r\n");
//                appSDData.state = APP_SD_ERROR;
//            } else {
//                /* File opened successfully */
//                UART3_WriteString33("SD: Source file opened OK\r\n");
//
//                /* Get file size */
//                int32_t fileSize = SYS_FS_FileSize(appSDData.fileHandle);
//                char sizeStr[12];
//                int_to_str(fileSize, sizeStr);
//                UART3_WriteString33("SD: File size: ");
//                UART3_WriteString33(sizeStr);
//                UART3_WriteString33(" bytes\r\n");
//
//                appSDData.state = APP_SD_CREATE_DIRECTORY;
//            }
//            break;
//        }
//
//        case APP_SD_CREATE_DIRECTORY:
//        {
//            UART3_WriteString33("SD: Creating directory: ");
//            UART3_WriteString33(SDCARD_DIR_NAME);
//            UART3_WriteString33("\r\n");
//
//            /* Create directory for destination file */
//            if (SYS_FS_DirectoryMake(SDCARD_DIR_NAME) == SYS_FS_RES_FAILURE) {
//                /* Directory creation failed (might already exist) */
//                UART3_WriteString33("SD: Directory create failed (may exist)\r\n");
//            } else {
//                UART3_WriteString33("SD: Directory created OK\r\n");
//            }
//
//            /* Proceed to open destination file */
//            appSDData.state = APP_SD_OPEN_SECOND_FILE;
//            break;
//        }
//
//        case APP_SD_OPEN_SECOND_FILE:
//        {
//            UART3_WriteString33("SD: Opening dest file: ");
//            UART3_WriteString33(SDCARD_DIR_NAME);
//            UART3_WriteString33("/");
//            UART3_WriteString33(SDCARD_FILE_NAME);
//            UART3_WriteString33("\r\n");
//
//            /* Open destination file for writing inside Dir1 */
//            appSDData.fileHandle1 = SYS_FS_FileOpen(
//                    SDCARD_DIR_NAME "/" SDCARD_FILE_NAME,
//                    SYS_FS_FILE_OPEN_WRITE
//                    );
//
//            if (appSDData.fileHandle1 == SYS_FS_HANDLE_INVALID) {
//                /* Could not create destination file */
//                UART3_WriteString33("SD: ERROR - Cannot create dest file!\r\n");
//                SYS_FS_FileClose(appSDData.fileHandle);
//                appSDData.state = APP_SD_ERROR;
//            } else {
//                /* Both files open */
//                UART3_WriteString33("SD: Dest file created OK\r\n");
//                UART3_WriteString33("SD: Starting copy...\r\n");
//                appSDData.state = APP_SD_READ_WRITE_TO_FILE;
//            }
//            break;
//        }
//
//        case APP_SD_READ_WRITE_TO_FILE:
//        {
//            /* Read from source file */
//            appSDData.nBytesRead = SYS_FS_FileRead(
//                    appSDData.fileHandle,
//                    (void *) readWriteBuffer,
//                    APP_SD_DATA_LEN
//                    );
//
//            if (appSDData.nBytesRead == -1) {
//                /* Read error */
//                UART3_WriteString33("SD: ERROR - Read failed!\r\n");
//                SYS_FS_FileClose(appSDData.fileHandle);
//                SYS_FS_FileClose(appSDData.fileHandle1);
//                appSDData.state = APP_SD_ERROR;
//            } else {
//                /* Write to destination file */
//                if (SYS_FS_FileWrite(appSDData.fileHandle1, (const void *) readWriteBuffer, appSDData.nBytesRead) == -1) {
//                    /* Write error */
//                    UART3_WriteString33("SD: ERROR - Write failed!\r\n");
//                    SYS_FS_FileClose(appSDData.fileHandle);
//                    SYS_FS_FileClose(appSDData.fileHandle1);
//                    appSDData.state = APP_SD_ERROR;
//                } else if (SYS_FS_FileEOF(appSDData.fileHandle) == 1) {
//                    /* End of file reached, copy complete */
//                    char bytesStr[12];
//                    int_to_str(appSDData.nBytesRead, bytesStr);
//                    UART3_WriteString33("SD: Last chunk: ");
//                    UART3_WriteString33(bytesStr);
//                    UART3_WriteString33(" bytes\r\n");
//                    UART3_WriteString33("SD: Copy complete!\r\n");
//                    appSDData.state = APP_SD_CLOSE_FILE;
//                } else {
//                    /* Print progress every chunk */
//                    static uint32_t totalBytes = 0;
//                    totalBytes += appSDData.nBytesRead;
//
//                    /* Print every 5KB */
//                    if (totalBytes % 5120 < 512) {
//                        char totalStr[12];
//                        int_to_str(totalBytes, totalStr);
//                        UART3_WriteString33("SD: Copied ");
//                        UART3_WriteString33(totalStr);
//                        UART3_WriteString33(" bytes...\r\n");
//                    }
//                }
//            }
//            break;
//        }
//
//        case APP_SD_CLOSE_FILE:
//        {
//            UART3_WriteString33("SD: Closing files...\r\n");
//
//            /* Close both files */
//            SYS_FS_FileClose(appSDData.fileHandle);
//            SYS_FS_FileClose(appSDData.fileHandle1);
//
//            UART3_WriteString33("SD: Files closed\r\n");
//
//            /* Operation successful, go to idle state */
//            appSDData.state = APP_SD_IDLE;
//            break;
//        }
//
//        case APP_SD_IDLE:
//        {
//            /* SD card operation completed successfully */
//            UART3_WriteString33("\r\n*** SD COPY SUCCESS! ***\r\n");
//            UART3_WriteString33("File copied to: ");
//            UART3_WriteString33(SDCARD_DIR_NAME);
//            UART3_WriteString33("/");
//            UART3_WriteString33(SDCARD_FILE_NAME);
//            UART3_WriteString33("\r\n");
//
//            /* Turn OFF LED to indicate success */
//            LED1_Off();
//            UART3_WriteString33("LED turned OFF (success)\r\n\n");
//
//            /* Stay in idle state - don't print again */
//            appSDData.state = APP_SD_IDLE + 100; // Move to quiet idle state
//            break;
//        }
//
//        case APP_SD_ERROR:
//        {
//            /* SD card operation failed */
//            UART3_WriteString33("\r\n*** SD ERROR STATE ***\r\n");
//            UART3_WriteString33("Check:\r\n");
//            UART3_WriteString33("1. SD card inserted\r\n");
//            UART3_WriteString33("2. SD card formatted (FAT32)\r\n");
//            UART3_WriteString33("3. Source file exists in root\r\n");
//            UART3_WriteString33("4. SD card connections correct\r\n\n");
//
//            /* Keep LED ON to indicate error */
//            LED1_On();
//
//            /* Stay in error state - don't print again */
//            appSDData.state = APP_SD_ERROR + 100; // Move to quiet error state
//            break;
//        }
//
//        default:
//        {
//            /* Quiet idle or error states - do nothing */
//            break;
//        }
//    }
//}
//
//
///*******************************************************************************
// End of File
// */
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
/////*******************************************************************************
////  MPLAB Harmony Application Source File
////
////  Company:
////    Microchip Technology Inc.
////
////  File Name:
////    app.c
////
////  Summary:
////    This file contains the source code for the MPLAB Harmony application.
////
////  Description:
////    This file contains the source code for the MPLAB Harmony application.  It
////    implements the logic of the application's state machine and it may call
////    API routines of other MPLAB Harmony modules in the system, such as drivers,
////    system services, and middleware.  However, it does not call any of the
////    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
////    the modules in the system or make any assumptions about when those functions
////    are called.  That is the responsibility of the configuration-specific system
////    files.
//// *******************************************************************************/
////
//////DOM-IGNORE-BEGIN
/////*******************************************************************************
////* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
////*
////* Subject to your compliance with these terms, you may use Microchip software
////* and any derivatives exclusively with Microchip products. It is your
////* responsibility to comply with third party license terms applicable to your
////* use of third party software (including open source software) that may
////* accompany Microchip software.
////*
////* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
////* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
////* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
////* PARTICULAR PURPOSE.
////*
////* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
////* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
////* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
////* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
////* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
////* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
////* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
////*******************************************************************************/
//////DOM-IGNORE-END
////
////// *****************************************************************************
////// *****************************************************************************
////// Section: Included Files
////// *****************************************************************************
////// *****************************************************************************
////
////#include "app.h"
////#include "bsp.h"
////#include "user.h"
////// *****************************************************************************
////// *****************************************************************************
////// Section: Global Data Definitions
////// *****************************************************************************
////// *****************************************************************************
////
////#define SDCARD_MOUNT_NAME    "/mnt/mydrive"
////#define SDCARD_DEV_NAME      "/dev/mmcblka1"
////#define SDCARD_FILE_NAME     "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG"
////#define SDCARD_DIR_NAME      "Dir1"
////
////#define APP_DATA_LEN         512
////
////// *****************************************************************************
/////* Application Data
////
////  Summary:
////    Holds application data
////
////  Description:
////    This structure holds the application's data.
////
////  Remarks:
////    This structure should be initialized by the APP_Initialize function.
////
////    Application strings and buffers are be defined outside this structure.
////*/
////
////APP_DATA appData;
////
////static uint8_t BUFFER_ATTRIBUTES readWriteBuffer[APP_DATA_LEN];
////
////// *****************************************************************************
////// *****************************************************************************
////// Section: Application Callback Functions
////// *****************************************************************************
////// *****************************************************************************
////
/////* TODO:  Add any necessary callback functions.
////*/
////
////// *****************************************************************************
////// *****************************************************************************
////// Section: Application Local Functions
////// *****************************************************************************
////// *****************************************************************************
////
////
/////* TODO:  Add any necessary local functions.
////*/
////
////
////// *****************************************************************************
////// *****************************************************************************
////// Section: Application Initialization and State Machine Functions
////// *****************************************************************************
////// *****************************************************************************
////
/////*******************************************************************************
////  Function:
////    void APP_Initialize ( void )
////
////  Remarks:
////    See prototype in app.h.
//// */
////
////void APP_Initialize ( void )
////{
////    /* Place the App state machine in its initial state. */
////    appData.state = APP_WAIT_SWITCH_PRESS; 
////    
////    /* Wait for the switch status to return to the default "not pressed" state */
//////    while(SWITCH_GET() == SWITCH_STATUS_PRESSED);
////}
////
////
/////******************************************************************************
////  Function:
////    void APP_Tasks ( void )
////
////  Remarks:
////    See prototype in app.h.
//// */
////
////void APP_Tasks ( void )
////{
////    //LED_ON();
////    /* Check the application's current state. */
////    switch ( appData.state )
////    {
////        case APP_WAIT_SWITCH_PRESS:
////            if (1 /*SWITCH_GET() == SWITCH_STATUS_PRESSED*/)
////            {
////                appData.state = APP_MOUNT_DISK;
////            }
////            break;
////        case APP_MOUNT_DISK:
////            if(SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
////            {
////                /* The disk could not be mounted. Try
////                 * mounting again until success. */
////                appData.state = APP_MOUNT_DISK;
////            }
////            else
////            {
////                /* Mount was successful. Unmount the disk, for testing. */
////                appData.state = APP_UNMOUNT_DISK;
////            }
////            break;
////
////        case APP_UNMOUNT_DISK:
////            if(SYS_FS_Unmount(SDCARD_MOUNT_NAME) != 0)
////            {
////                /* The disk could not be un mounted. Try
////                 * un mounting again untill success. */
////
////                appData.state = APP_UNMOUNT_DISK;
////            }
////            else
////            {
////                /* UnMount was successful. Mount the disk again */
////                appData.state = APP_MOUNT_DISK_AGAIN;
////            }
////            break;
////
////        case APP_MOUNT_DISK_AGAIN:
////            if(SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
////            {
////                /* The disk could not be mounted. Try
////                 * mounting again until success. */
////                appData.state = APP_MOUNT_DISK_AGAIN;
////            }
////            else
////            {
////                /* Mount was successful. Set current drive so that we do not have to use absolute path. */
////                appData.state = APP_SET_CURRENT_DRIVE;
////            }
////            break;
////
////        case APP_SET_CURRENT_DRIVE:
////            if(SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
////            {
////                /* Error while setting current drive */
////                appData.state = APP_ERROR;
////            }
////            else
////            {
////                /* Open a file for reading. */
////                appData.state = APP_OPEN_FIRST_FILE;
////            }
////            break;
////
////        case APP_OPEN_FIRST_FILE:
////            appData.fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, (SYS_FS_FILE_OPEN_READ));
////            if(appData.fileHandle == SYS_FS_HANDLE_INVALID)
////            {
////                /* Could not open the file. Error out*/
////                appData.state = APP_ERROR;
////            }
////            else
////            {
////                /* Create a directory. */
////                appData.state = APP_CREATE_DIRECTORY;
////            }
////            break;
////
////        case APP_CREATE_DIRECTORY:
////            if(SYS_FS_DirectoryMake(SDCARD_DIR_NAME) == SYS_FS_RES_FAILURE)
////            {
////                /* Error while creating a new drive */
////                appData.state = APP_ERROR;
////            }
////            else
////            {
////                /* Open a second file for writing. */
////                appData.state = APP_OPEN_SECOND_FILE;
////            }
////            break;
////
////        case APP_OPEN_SECOND_FILE:
////            /* Open a second file inside "Dir1" */
////            appData.fileHandle1 = SYS_FS_FileOpen(SDCARD_DIR_NAME"/"SDCARD_FILE_NAME,
////                    (SYS_FS_FILE_OPEN_WRITE));
////                       
////            if(appData.fileHandle1 == SYS_FS_HANDLE_INVALID)
////            {
////                /* Could not open the file. Error out*/
////                appData.state = APP_ERROR;
////            }
////            else
////            {                
////                /* Read from one file and write to another file */
////                appData.state = APP_READ_WRITE_TO_FILE;
////            }
////            break;
////        case APP_READ_WRITE_TO_FILE:
////            
////            appData.nBytesRead = SYS_FS_FileRead(appData.fileHandle, (void *)readWriteBuffer, APP_DATA_LEN);
////            
////            if (appData.nBytesRead == -1)
////            {
////                /* There was an error while reading the file.
////                 * Close the file and error out. */
////                SYS_FS_FileClose(appData.fileHandle);
////                appData.state = APP_ERROR;
////            }
////            else
////            {
////                /* If read was success, try writing to the new file */
////                if(SYS_FS_FileWrite(appData.fileHandle1, (const void *)readWriteBuffer, appData.nBytesRead) == -1)
////                {                    
////                    /* Write was not successful. Close the file
////                     * and error out.*/
////                    SYS_FS_FileClose(appData.fileHandle1);
////                    appData.state = APP_ERROR;
////                }
////                else if(SYS_FS_FileEOF(appData.fileHandle) == 1)    /* Test for end of file */
////                {                                        
////                    /* Continue the read and write process, until the end of file is reached */
////                    appData.state = APP_CLOSE_FILE;
////                }
////            }
////            break;
////
////        case APP_CLOSE_FILE:
////            /* Close both files */
////            SYS_FS_FileClose(appData.fileHandle);
////            SYS_FS_FileClose(appData.fileHandle1);
////
////            /* The test was successful. Lets idle. */
////            appData.state = APP_IDLE;
////            break;
////
////        case APP_IDLE:
////            /* The application comes here when the demo has completed
////             * successfully. Glow LED1. */
////            LED_ON();
////            break;
////
////        case APP_ERROR:
////            /* The application comes here when the demo has failed. */
////            break;
////
////        default:
////            break;
////    }
////}
////
////
////
/////*******************************************************************************
//// End of File
//// */
