#ifndef SD_SERVICE_H
#define SD_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Call once at boot */
void SD_Service_Init(void);

/* Call periodically (e.g., from a protothread) until it reports mounted */
void SD_Service_Task(void);

/* True once mounted */
bool SD_Service_IsMounted(void);

/* Returns Harmony mount point string (e.g. "/mnt/myDrive1") */
const char* SD_Service_MountPoint(void);

#ifdef __cplusplus
}
#endif

#endif /* SD_SERVICE_H */
