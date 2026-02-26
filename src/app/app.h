/**
 * @file    app.h
 * @brief   Application layer — top-level orchestrator.
 *
 * @details Provides two entry points called from main():
 *            App_Init()  — initialise all layers in dependency order
 *            App_Run()   — enter the cooperative Protothread scheduler (never returns)
 *
 *          Harmony's generated code calls APP_Initialize() and APP_Tasks().
 *          These are thin stubs — all real work runs through Protothreads.
 */

#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ══════════════════════════════════════════════════════════════════════════
 *  Harmony compatibility  (called from initialization.c / tasks.c)
 *
 *  These must exist so the linker is happy.  They are intentionally empty
 *  because all application logic runs inside Protothreads.
 * ══════════════════════════════════════════════════════════════════════════ */
void APP_Initialize(void);
void APP_Tasks(void);

/* ══════════════════════════════════════════════════════════════════════════
 *  Custom orchestrator (called from main.c)
 * ══════════════════════════════════════════════════════════════════════════ */

/**
 * @brief  Initialise the entire system (L0 → L4).
 */
void App_Init(void);

/**
 * @brief  Run the main cooperative scheduler loop.  Never returns.
 *         Calls SYS_Tasks() and all active Protothreads.
 */
void App_Run(void);

#endif /* APP_H */
