/*******************************************************************************
  SD Card Thread Implementation
  Replaces app.c state machine with protothread
 *******************************************************************************/
//#include "lc.h"
#include "bsp.h"
#include <string.h>
#include "definitions.h"
#include "sd_card_thread.h"


// Configuration (same as original app.c)
// Make buffer static to avoid stack usage
static char path[64];  // Was on stack, now static
#define SDCARD_MOUNT_NAME    "/mnt/mydrive"
#define SDCARD_DEV_NAME      "/dev/mmcblka1"
#define SDCARD_FILE_NAME     "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG"
#define SDCARD_DIR_NAME      "Dir1"

#define APP_DATA_LEN         512

// Thread state
//static struct pt ptSDCard;
static uint8_t readWriteBuffer[APP_DATA_LEN];

// Status tracking
static bool sd_ready = false;
static const char* status_msg = "INIT";

// Static file handles (persist across yields)
static SYS_FS_HANDLE fileHandle = SYS_FS_HANDLE_INVALID;
static SYS_FS_HANDLE fileHandle1 = SYS_FS_HANDLE_INVALID;

// Timeout tracking
static uint32_t timeout_start;

// Helper: Start timeout
#define TIMEOUT_START(ms)   timeout_start = msTicks
#define TIMEOUT_EXPIRED(ms) ((msTicks - timeout_start) >= (ms))

// Helper: Check if mount succeeded (non-blocking check)
static bool check_mount_ready(void)
{
    // Try to set current drive - if mount is ready, this succeeds
    return (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_SUCCESS);
}

PT_THREAD(SDCardThread(struct pt *pt))
{
    PT_BEGIN(pt);
    
    // State: Mount disk
    status_msg = "MOUNTING";
    TIMEOUT_START(5000);  // 5 second timeout
    
    while (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
    {
        PT_YIELD(pt);  // Cooperative yield
        
        if (TIMEOUT_EXPIRED(5000))
        {
            status_msg = "MOUNT_TIMEOUT";
            PT_EXIT(pt);  // Fatal error
        }
    }
    
    // Optional: Test unmount/remount (from original app.c)
    PT_YIELD(pt);
    
    SYS_FS_Unmount(SDCARD_MOUNT_NAME);
    PT_YIELD(pt);
    
    // Remount
    TIMEOUT_START(5000);
    while (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0)
    {
        PT_YIELD(pt);
        if (TIMEOUT_EXPIRED(5000))
        {
            status_msg = "REMOUNT_TIMEOUT";
            PT_EXIT(pt);
        }
    }
    
    // Set current drive
    PT_YIELD(pt);
    if (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE)
    {
        status_msg = "SET_DRIVE_FAIL";
        PT_EXIT(pt);
    }
    
    // Open source file (or create if doesn't exist)
    status_msg = "OPEN_SRC";
    PT_YIELD(pt);
    
    fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, SYS_FS_FILE_OPEN_READ);
    
    // If source doesn't exist, we'll create it during copy phase
    // (simplified: just proceed to create directory)
    
    // Create directory
    status_msg = "MKDIR";
    PT_YIELD(pt);
    SYS_FS_DirectoryMake(SDCARD_DIR_NAME);  // Ignore error (may exist)
    
    // Open destination file
    status_msg = "OPEN_DST";
    PT_YIELD(pt);
    
    char path[64];
    strncpy(path, SDCARD_DIR_NAME, sizeof(path)-1);
    strncat(path, "/", sizeof(path)-strlen(path)-1);
    strncat(path, SDCARD_FILE_NAME, sizeof(path)-strlen(path)-1);
    
    fileHandle1 = SYS_FS_FileOpen(path, SYS_FS_FILE_OPEN_WRITE);
    
    if (fileHandle1 == SYS_FS_HANDLE_INVALID)
    {
        if (fileHandle != SYS_FS_HANDLE_INVALID)
        {
            SYS_FS_FileClose(fileHandle);
            fileHandle = SYS_FS_HANDLE_INVALID;
        }
        status_msg = "OPEN_DST_FAIL";
        PT_EXIT(pt);
    }
    
    // Copy data with periodic yields
    status_msg = "COPYING";
    PT_YIELD(pt);
    
    if (fileHandle == SYS_FS_HANDLE_INVALID)
    {
        // No source file - write test message
        const char* testMsg = "Hello from PIC32MM SD Card via Protothread!\r\n";
        SYS_FS_FileWrite(fileHandle1, testMsg, strlen(testMsg));
    }
    else
    {
        // Copy loop with yields every 512 bytes
        int32_t nBytes;
        do
        {
            nBytes = SYS_FS_FileRead(fileHandle, readWriteBuffer, APP_DATA_LEN);
            
            if (nBytes > 0)
            {
                if (SYS_FS_FileWrite(fileHandle1, readWriteBuffer, nBytes) == -1)
                {
                    status_msg = "WRITE_ERROR";
                    break;
                }
            }
            
            // Yield every block to keep system responsive
            PT_YIELD(pt);
            
        } while (nBytes > 0);
    }
    
    // Close files
    status_msg = "CLOSING";
    PT_YIELD(pt);
    
    if (fileHandle != SYS_FS_HANDLE_INVALID)
    {
        SYS_FS_FileClose(fileHandle);
        fileHandle = SYS_FS_HANDLE_INVALID;
    }
    
    if (fileHandle1 != SYS_FS_HANDLE_INVALID)
    {
        SYS_FS_FileClose(fileHandle1);
        fileHandle1 = SYS_FS_HANDLE_INVALID;
    }
    
    // Success
    sd_ready = true;
    status_msg = "READY";
    
    // Idle state - keep thread alive but do nothing
    while (1)
    {
        PT_YIELD(pt);
    }
    
    PT_END(pt);
}

// Optional API functions
bool SDCard_IsReady(void)
{
    return sd_ready;
}

const char* SDCard_GetStatus(void)
{
    return status_msg;
}

void SDCard_Restart(void)
{
    PT_INIT(&ptSDCard);
    sd_ready = false;
    status_msg = "RESTART";//
}