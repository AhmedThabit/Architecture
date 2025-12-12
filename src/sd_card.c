#include "ff.h"
#include "diskio.h"
#include "sd_spi_lowlevel.h"
#include "definitions.h"

static FATFS g_fs;

bool SD_CardInitAndMount(void)
{
    if (disk_initialize(0) & STA_NOINIT) {
        UART3_WriteString33("SD init failed\r\n");
        return false;
    }

    FRESULT fr = f_mount(&g_fs, "", 1);
    if (fr != FR_OK) {
        UART3_WriteString33("f_mount failed\r\n");
        return false;
    }

    UART3_WriteString33("SD card mounted\r\n");
    return true;
}

void SD_Test_ReadAMR(void)
{
    FIL f;
    FRESULT fr;
    UINT br;
    BYTE buf[128];

    fr = f_open(&f, "voice1.amr", FA_READ);
    if (fr != FR_OK) {
        UART3_WriteString33("Failed to open voice1.amr\r\n");
        return;
    }

    UART3_WriteString33("Reading first 128 bytes of voice1.amr:\r\n");

    fr = f_read(&f, buf, sizeof(buf), &br);
    if (fr != FR_OK) {
        UART3_WriteString33("f_read error\r\n");
        f_close(&f);
        return;
    }

    for (UINT i = 0; i < br; i++) {
        uart3_write_hex(buf[i]);
        UART3_WriteString33(" ");
    }
    UART3_WriteString33("\r\nDone.\r\n");

    f_close(&f);
}
