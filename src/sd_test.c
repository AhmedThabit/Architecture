#include "sd_test.h"

#include <string.h>
#include "definitions.h"
#include "sd_service.h"

/* Your existing UART helper */
extern void BlockingUART3_WriteString33(const char* s);

static void uart3_puts(const char* s)
{
    BlockingUART3_WriteString33(s);
}

/* Minimal hex */
static void uart3_hex8(uint8_t b)
{
    const char hex[] = "0123456789ABCDEF";
    char out[3] = {hex[b>>4], hex[b&0xF], 0};
    uart3_puts(out);
}

PT_THREAD(SD_TestThread(struct pt *pt))
{
    static SYS_FS_DIR_HANDLE dir;
    static SYS_FS_FSTAT stat;
    static char path[96];

    PT_BEGIN(pt);

    uart3_puts("SD_TEST: init\r\n");

    uart3_puts("SD_TEST: waiting for sd_mounted...\r\n");
    PT_WAIT_UNTIL(pt, SD_Service_IsMounted());

    uart3_puts("SD_TEST: sd_mounted=true\r\n");

    /* Build root path like "/mnt/myDrive1/" */
    strncpy(path, SD_Service_MountPoint(), sizeof(path)-2);
    path[sizeof(path)-2] = '\0';
    size_t n = strlen(path);
    if (n == 0 || path[n-1] != '/')
    {
        path[n++] = '/';
        path[n] = '\0';
    }

    uart3_puts("SD_TEST: DirOpen: ");
    uart3_puts(path);
    uart3_puts("\r\n");

    dir = SYS_FS_DirOpen(path);
    if (dir == SYS_FS_HANDLE_INVALID)
    {
        uart3_puts("SD_TEST: DirOpen FAILED\r\n");
        PT_END(pt);
    }

    uart3_puts("SD_TEST: listing...\r\n");

    while (SYS_FS_DirRead(dir, &stat) == 0)
    {
        /* Skip empty names */
        if (stat.fname[0] == '\0')
            continue;

        uart3_puts("  ");
        uart3_puts(stat.fname);
        uart3_puts("  size=");
        /* size is uint32/uint64 depending on config; print low byte only for quick debug */
        uart3_hex8((uint8_t)(stat.fsize & 0xFFu));
        uart3_puts("\r\n");

        PT_YIELD(pt); /* yield to keep system responsive during long directory listings */
    }

    SYS_FS_DirClose(dir);
    uart3_puts("SD_TEST: DONE\r\n");

    while (1) PT_YIELD(pt);

    PT_END(pt);
}
