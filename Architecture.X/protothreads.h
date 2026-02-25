/**
 * @file    protothreads.h
 * @brief   Protothread declarations and control blocks.
 */

#ifndef PROTOTHREADS_H
#define PROTOTHREADS_H

#include "common/pt.h"

/* ── Protothread control blocks ─────────────────────────────────────────── */

extern struct pt ptSensor;
extern struct pt ptTelit;
extern struct pt ptEsp32;
extern struct pt ptEspTxTest;
extern struct pt ptEth;
extern struct pt ptCLI;
extern struct pt ptPreflight;
extern struct pt ptSdCard;

/* ── Thread function declarations ───────────────────────────────────────── */

PT_THREAD(SensorThread(struct pt *pt));
PT_THREAD(TelitThread(struct pt *pt));
PT_THREAD(Esp32Thread(struct pt *pt));
PT_THREAD(Esp32TxTestThread(struct pt *pt));
PT_THREAD(EthThread(struct pt *pt));
PT_THREAD(CliThread(struct pt *pt));
PT_THREAD(TelitPreflightThread(struct pt *pt));
PT_THREAD(SdCardThread(struct pt *pt));

/** Initialise all protothread control blocks.  Call once before App_Run(). */
void Protothreads_Init(void);

#endif /* PROTOTHREADS_H */
