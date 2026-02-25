/**
 * @file    app.h
 * @brief   Application layer — top-level orchestrator.
 *
 * @details Provides two entry points called from main():
 *            App_Init()  — initialise all layers in dependency order
 *            App_Run()   — enter the cooperative Protothread scheduler (never returns)
 *
 *          Also provides the APP_DATA state machine used by Harmony's
 *          SYS_Tasks() → APP_Tasks() call chain for SD card operations.
 */

#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "system/fs/sys_fs.h"

/* ══════════════════════════════════════════════════════════════════════════
 *  SD Card State Machine  (driven by APP_Tasks(), called from SYS_Tasks())
 * ══════════════════════════════════════════════════════════════════════════ */

typedef enum {
    APP_WAIT_SWITCH_PRESS,
    APP_MOUNT_DISK,
    APP_UNMOUNT_DISK,
    APP_MOUNT_DISK_AGAIN,
    APP_SET_CURRENT_DRIVE,
    APP_OPEN_FIRST_FILE,
    APP_CREATE_DIRECTORY,
    APP_OPEN_SECOND_FILE,
    APP_READ_WRITE_TO_FILE,
    APP_CLOSE_FILE,
    APP_IDLE,
    APP_ERROR
} APP_STATES;

typedef struct {
    APP_STATES      state;
    SYS_FS_HANDLE   fileHandle;
    SYS_FS_HANDLE   fileHandle1;
    int32_t         nBytesRead;
} APP_DATA;

extern APP_DATA appData;

/* Harmony calls these from initialization.c / tasks.c */
void APP_Initialize(void);
void APP_Tasks(void);

/* ══════════════════════════════════════════════════════════════════════════
 *  Custom orchestrator (called from main.c)
 * ══════════════════════════════════════════════════════════════════════════ */

/**
 * @brief  Initialise the entire system.
 *
 *         Calls layers in order:
 *           L0  SYS_Initialize()        — Harmony MCAL
 *           L1  BSP_Initialize()        — board GPIO, LEDs
 *                BSP_Timer1_Init()      — 1 ms system tick
 *                SPIBus_Init()          — shared SPI bus guard
 *           L2  Flash_Init()            — W25Q32 external flash
 *                GSM_Service_Init()     — modem UART + AT engine
 *                ESP32_Comm_Init()      — ESP32 binary link
 *           L3  PhonebookFlash_Init()   — phonebook from flash
 *                Cfg_Load()             — device config from flash
 *           L4  Protothreads_Init()     — protothread control blocks
 */
void App_Init(void);

/**
 * @brief  Run the main cooperative scheduler loop.
 *         Never returns.  Calls SYS_Tasks() and all active Protothreads.
 */
void App_Run(void);

#endif /* APP_H */
