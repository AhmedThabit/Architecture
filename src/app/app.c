/**
 * @file    app.c
 * @brief   Application layer — system initialisation and scheduler.
 *
 * @details All init logic and the main loop live here, keeping main.c
 *          as a thin two-line entry point.
 *
 *          APP_Initialize() and APP_Tasks() are empty stubs required by
 *          Harmony's generated initialization.c / tasks.c.
 *          All real application logic runs inside Protothreads.
 */

#include "app.h"
#include "config/app_config.h"

/* ── Harmony compatibility stubs ─────────────────────────────────────────
 *  Harmony's initialization.c calls APP_Initialize() and tasks.c calls
 *  APP_Tasks() every pass through SYS_Tasks().
 *
 *  These are empty because all application work (including SD card) runs
 *  through Protothreads in App_Run().  SYS_Tasks() still services the
 *  Harmony drivers (SYS_FS_Tasks, DRV_SDSPI_Tasks) which the SD card
 *  protothread depends on.
 * ────────────────────────────────────────────────────────────────────── */
void APP_Initialize(void) { /* nothing — App_Init() does the real work */ }
void APP_Tasks(void)      { /* nothing — SdCardThread drives SD card   */ }

/* L0 — MCAL (Harmony) */
#include "definitions.h"

/* L1 — HAL */
#include "hal/bsp/bsp.h"
#include "hal/spi_guard/spi_bus_guard.h"
#include "hal/io_monitor/io_monitor.h"

/* L2 — Drivers */
#include "drivers/flash/flash_w25q32.h"
#include "drivers/modem/modem_uart.h"
#include "drivers/modem/modem_at.h"

/* L3 — Services */
#include "services/modem_api/modem_api.h"
#include "services/storage/storage.h"
#include "services/esp32_proto/esp32_proto.h"
#include "services/alarm/alarm_mgr.h"
#include "common/schema.h"

/* Protothread scheduler */
#include "common/pt.h"

/* Forward declarations from protothreads.c */
extern struct pt ptTelit, ptEsp32, ptSensor, ptEth, ptCLI, ptPreflight, ptSdCard;
extern struct pt ptAudio, ptAlarm;
extern void Protothreads_Init(void);

PT_THREAD(TelitThread(struct pt *pt));
PT_THREAD(Esp32Thread(struct pt *pt));
PT_THREAD(SensorThread(struct pt *pt));
PT_THREAD(EthThread(struct pt *pt));
PT_THREAD(CliThread(struct pt *pt));
PT_THREAD(TelitPreflightThread(struct pt *pt));
PT_THREAD(SdCardThread(struct pt *pt));
PT_THREAD(AudioThread(struct pt *pt));
PT_THREAD(AlarmThread(struct pt *pt));

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
    /* IO_Init() is called AFTER Cfg_Load() so it can apply the persisted
     * channel functions (input/output/analogue) saved in flash. */

    /* ── L2: Drivers (external chips) ───────────────────────────────── */
    Flash_Init();
    Modem_Init();               /* modem UART + AT engine            */
    ESP32_UartInit();           /* ESP32 binary link                 */
    ESP32_RegisterFrameHandler(Esp_HandleFrame);

    /* ── L3: Services (business logic using drivers) ────────────────── */
    PhonebookFlash_Init();
    Cfg_Load();

    /* Now configure GPIO pins according to the loaded config.
     * This ensures channels saved as Output (Off/On) actually drive their
     * pins to the correct level on boot, not just after the user re-saves. */
    IO_Init();

    /* ── L4: Application (scheduler) ────────────────────────────────── */
    Protothreads_Init();

    /* Enable global interrupts last */
    (void)__builtin_enable_interrupts();
}

/* ══════════════════════════════════════════════════════════════════════════
 *  App_Run — Cooperative Protothread scheduler (never returns)
 *
 *  SYS_Tasks() runs Harmony internals every pass:
 *    SYS_FS_Tasks()          — file-system state machine
 *    DRV_SDSPI_Tasks()       — SD SPI driver state machine
 *    APP_Tasks()             — empty stub (our code is in protothreads)
 * ══════════════════════════════════════════════════════════════════════════ */

void App_Run(void)
{
    while (1) {
        /* Maintain Harmony state machines (file system, SD SPI driver) */
        SYS_Tasks();

        /* Schedule active Protothreads */
        PT_SCHEDULE(Esp32Thread(&ptEsp32));
        PT_SCHEDULE(SdCardThread(&ptSdCard));
        PT_SCHEDULE(TelitPreflightThread(&ptPreflight));
        PT_SCHEDULE(AudioThread(&ptAudio));
        PT_SCHEDULE(AlarmThread(&ptAlarm));

        /*
         * Uncomment as you bring up each subsystem:
         *
         * PT_SCHEDULE(TelitThread(&ptTelit));
         * PT_SCHEDULE(SensorThread(&ptSensor));
         * PT_SCHEDULE(EthThread(&ptEth));
         * PT_SCHEDULE(CliThread(&ptCLI));
         */
    }
}
