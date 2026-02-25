/**
 * @file    app.c
 * @brief   Application layer — system initialisation, SD card state machine,
 *          and cooperative Protothread scheduler.
 *
 * @details All init logic and the main loop live here, keeping main.c
 *          as a thin two-line entry point.
 *
 *          The SD card file-copy demo runs inside APP_Tasks(), which Harmony's
 *          tasks.c calls every pass through SYS_Tasks().  This is the same
 *          state machine that was working before the refactoring.
 */

#include "app.h"
#include "config/app_config.h"
#include "user.h"

/* L0 — MCAL (Harmony) */
#include "definitions.h"

/* L1 — HAL */
#include "hal/bsp/bsp.h"
#include "hal/spi_guard/spi_bus_guard.h"

/* L2 — Drivers */
#include "drivers/flash/flash_w25q32.h"
#include "drivers/modem/modem_uart.h"
#include "drivers/modem/modem_at.h"

/* L3 — Services */
#include "services/gsm/gsm_service.h"
#include "services/storage/storage.h"
#include "services/esp32_proto/esp32_proto.h"
#include "common/schema.h"

/* Protothread scheduler */
#include "common/pt.h"

/* ══════════════════════════════════════════════════════════════════════════
 *  SD Card Constants
 * ══════════════════════════════════════════════════════════════════════════ */

#define SDCARD_MOUNT_NAME    "/mnt/mydrive"
#define SDCARD_DEV_NAME      "/dev/mmcblka1"
#define SDCARD_FILE_NAME     "FILE_TOO_LONG_NAME_EXAMPLE_123.JPEG"
#define SDCARD_DIR_NAME      "Dir1"
#define APP_DATA_LEN         512

/* ══════════════════════════════════════════════════════════════════════════
 *  SD Card State Machine Data
 * ══════════════════════════════════════════════════════════════════════════ */

APP_DATA appData;

static uint8_t BUFFER_ATTRIBUTES readWriteBuffer[APP_DATA_LEN];

extern void UART3_WriteString33(const char *str);

/* ══════════════════════════════════════════════════════════════════════════
 *  APP_Initialize — Called by Harmony's initialization.c
 *
 *  Sets the SD card state machine to its initial state.
 * ══════════════════════════════════════════════════════════════════════════ */

void APP_Initialize(void)
{
    LED_OFF();
    appData.state = APP_WAIT_SWITCH_PRESS;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  APP_Tasks — Called by Harmony's tasks.c every pass through SYS_Tasks()
 *
 *  This is the original working SD card state machine:
 *    Mount → Unmount → Remount → Set drive → Open file → Create dir →
 *    Open 2nd file → Read/Write copy → Close → LED ON (success)
 * ══════════════════════════════════════════════════════════════════════════ */

void APP_Tasks(void)
{
    switch (appData.state) {

        case APP_WAIT_SWITCH_PRESS:
            if (1 /* SWITCH_GET() == SWITCH_STATUS_PRESSED */) {
                appData.state = APP_MOUNT_DISK;
            }
            break;

        case APP_MOUNT_DISK:
            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0) {
                appData.state = APP_MOUNT_DISK;
            } else {
                appData.state = APP_UNMOUNT_DISK;
            }
            break;

        case APP_UNMOUNT_DISK:
            if (SYS_FS_Unmount(SDCARD_MOUNT_NAME) != 0) {
                appData.state = APP_UNMOUNT_DISK;
            } else {
                appData.state = APP_MOUNT_DISK_AGAIN;
            }
            break;

        case APP_MOUNT_DISK_AGAIN:
            if (SYS_FS_Mount(SDCARD_DEV_NAME, SDCARD_MOUNT_NAME, FAT, 0, NULL) != 0) {
                appData.state = APP_MOUNT_DISK_AGAIN;
            } else {
                appData.state = APP_SET_CURRENT_DRIVE;
            }
            break;

        case APP_SET_CURRENT_DRIVE:
            if (SYS_FS_CurrentDriveSet(SDCARD_MOUNT_NAME) == SYS_FS_RES_FAILURE) {
                appData.state = APP_ERROR;
            } else {
                appData.state = APP_OPEN_FIRST_FILE;
            }
            break;

        case APP_OPEN_FIRST_FILE:
            appData.fileHandle = SYS_FS_FileOpen(SDCARD_FILE_NAME, (SYS_FS_FILE_OPEN_READ));
            if (appData.fileHandle == SYS_FS_HANDLE_INVALID) {
                appData.state = APP_ERROR;
            } else {
                appData.state = APP_CREATE_DIRECTORY;
            }
            break;

        case APP_CREATE_DIRECTORY:
            if (SYS_FS_DirectoryMake(SDCARD_DIR_NAME) == SYS_FS_RES_FAILURE) {
                appData.state = APP_ERROR;
            } else {
                appData.state = APP_OPEN_SECOND_FILE;
            }
            break;

        case APP_OPEN_SECOND_FILE:
            appData.fileHandle1 = SYS_FS_FileOpen(SDCARD_DIR_NAME "/" SDCARD_FILE_NAME,
                                                   (SYS_FS_FILE_OPEN_WRITE));
            if (appData.fileHandle1 == SYS_FS_HANDLE_INVALID) {
                appData.state = APP_ERROR;
            } else {
                appData.state = APP_READ_WRITE_TO_FILE;
            }
            break;

        case APP_READ_WRITE_TO_FILE:
            appData.nBytesRead = SYS_FS_FileRead(appData.fileHandle,
                                                  (void *)readWriteBuffer, APP_DATA_LEN);
            if (appData.nBytesRead == -1) {
                SYS_FS_FileClose(appData.fileHandle);
                appData.state = APP_ERROR;
            } else {
                if (SYS_FS_FileWrite(appData.fileHandle1,
                                     (const void *)readWriteBuffer,
                                     appData.nBytesRead) == -1) {
                    SYS_FS_FileClose(appData.fileHandle1);
                    appData.state = APP_ERROR;
                } else if (SYS_FS_FileEOF(appData.fileHandle) == 1) {
                    appData.state = APP_CLOSE_FILE;
                }
            }
            break;

        case APP_CLOSE_FILE:
            SYS_FS_FileClose(appData.fileHandle);
            SYS_FS_FileClose(appData.fileHandle1);
            appData.state = APP_IDLE;
            break;

        case APP_IDLE:
            LED_ON();
            break;

        case APP_ERROR:
            /* Demo failed — stay here */
            break;

        default:
            break;
    }
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Forward declarations from protothreads.c
 * ══════════════════════════════════════════════════════════════════════════ */

extern struct pt ptTelit, ptEsp32, ptSensor, ptEth, ptCLI, ptPreflight, ptSdCard;
extern void Protothreads_Init(void);

PT_THREAD(TelitThread(struct pt *pt));
PT_THREAD(Esp32Thread(struct pt *pt));
PT_THREAD(SensorThread(struct pt *pt));
PT_THREAD(EthThread(struct pt *pt));
PT_THREAD(CliThread(struct pt *pt));
PT_THREAD(TelitPreflightThread(struct pt *pt));
PT_THREAD(SdCardThread(struct pt *pt));

/* External ESP32 init */
extern void ESP32_UartInit(void);
extern void ESP32_RegisterFrameHandler(void (*cb)(const uint8_t*, size_t));
extern void Esp_HandleFrame(const uint8_t* payload, size_t len);

/* Phonebook flash init */
extern void PhonebookFlash_Init(void);

/* ══════════════════════════════════════════════════════════════════════════
 *  App_Init — Initialise all layers in dependency order
 * ══════════════════════════════════════════════════════════════════════════ */

void App_Init(void)
{
    /* ── L0: MCAL (clocks, GPIO mux, UART, SPI, EVIC) ──────────────── */
    SYS_Initialize(NULL);

    /* ── L1: HAL (board-level hardware) ─────────────────────────────── */
    BSP_Initialize();
    BSP_Timer1_Init();
    SPIBus_Init();

    /* ── L2: Drivers (external chips) ───────────────────────────────── */
    Flash_Init();
    GSM_Service_Init();         /* modem UART + AT engine            */
    ESP32_UartInit();           /* ESP32 binary link                 */
    ESP32_RegisterFrameHandler(Esp_HandleFrame);

    /* ── L3: Services (business logic using drivers) ────────────────── */
    PhonebookFlash_Init();
    Cfg_Load();

    /* ── L4: Application (scheduler) ────────────────────────────────── */
    Protothreads_Init();

    /* Enable global interrupts last */
    (void)__builtin_enable_interrupts();
}

/* ══════════════════════════════════════════════════════════════════════════
 *  App_Run — Cooperative Protothread scheduler (never returns)
 *
 *  NOTE: SYS_Tasks() is called every iteration, which in turn calls
 *        APP_Tasks() — that's where the SD card state machine runs.
 * ══════════════════════════════════════════════════════════════════════════ */

void App_Run(void)
{
    while (1) {
        /* Maintain Harmony state machines (USB, file system, SD card, etc.)
         * This calls APP_Tasks() internally — the SD card state machine. */
        SYS_Tasks();

        /* Schedule active Protothreads */
        PT_SCHEDULE(Esp32Thread(&ptEsp32));

        /*
         * Uncomment as you bring up each subsystem:
         *
         * PT_SCHEDULE(TelitPreflightThread(&ptPreflight));
         * PT_SCHEDULE(TelitThread(&ptTelit));
         * PT_SCHEDULE(SensorThread(&ptSensor));
         * PT_SCHEDULE(EthThread(&ptEth));
         * PT_SCHEDULE(CliThread(&ptCLI));
         */
    }
}
