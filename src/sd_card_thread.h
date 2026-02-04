/*******************************************************************************
  SD Card Thread Header
  Protothread-based SD card file operations (replaces app.h/app.c)
 *******************************************************************************/

#ifndef SD_CARD_THREAD_H
#define SD_CARD_THREAD_H

//#include "pt.h"           // Protothreads
#include "../../pt.h"    // Protothreads core
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Thread control structure (declared in protothreads.h or main)
extern struct pt ptSDCard;
extern volatile uint32_t sd_timeout_start;  // If needed
// The SD card protothread - call this in main loop
PT_THREAD(SDCardThread(struct pt *pt));

// Optional: Check if SD card operations completed successfully
bool SDCard_IsReady(void);
const char* SDCard_GetStatus(void);

// Optional: Restart the SD card operation sequence
void SDCard_Restart(void);

#ifdef __cplusplus
}
#endif

#endif /* SD_CARD_THREAD_H */