#ifndef SD_TEST_H
#define SD_TEST_H

#include "../../pt.h"

#ifdef __cplusplus
extern "C" {
#endif

/* A simple one-shot SD test protothread:
 * - waits for SD_Service_IsMounted()
 * - lists the root directory of the mount point
 */
PT_THREAD(SD_TestThread(struct pt *pt));

#ifdef __cplusplus
}
#endif

#endif /* SD_TEST_H */
