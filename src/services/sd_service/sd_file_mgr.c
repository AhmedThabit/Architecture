/**
 * @file    sd_file_mgr.c
 * @brief   SD card file manager — deferred execution from SdCardThread.
 *
 * @details The TLV handler (esp32_proto.c) posts requests via FileMgr_Post().
 *          The SdCardThread calls FileMgr_Process() each loop, which does
 *          the actual SYS_FS work and sends the result back via ESP32.
 *
 *          This avoids calling SYS_FS from within Esp_HandleFrame(),
 *          which would hang because SYS_Tasks() can't run.
 */

#include "sd_file_mgr.h"
#include "sd_service.h"
#include "definitions.h"

/* FatFS direct access for f_getfree */
#include "ff.h"

#include <string.h>
#include <stdio.h>

/* For sending the response back */
extern bool ESP32_SendFrame(const uint8_t *payload, size_t len);

/* Debug */
extern void BlockingUART3_WriteString33(const char *s);

/* Mount name — must match SdCardThread */
#define FM_MOUNT_NAME   "/mnt/mydrive"

/* ══════════════════════════════════════════════════════════════════════════
 *  Request queue (single slot — one operation at a time)
 * ══════════════════════════════════════════════════════════════════════════ */

#define FM_PATH_MAX  128
#define FM_RSP_MAX   400

static volatile bool    s_pending = false;
static uint8_t          s_op = 0;
static char             s_path1[FM_PATH_MAX];   /* primary path or data */
static char             s_path2[FM_PATH_MAX];   /* second path (rename) */
static size_t           s_data_len = 0;

/* Response buffer */
static uint8_t          s_rsp[FM_RSP_MAX];

/* ── Strip leading slashes to make path relative ────────────────────────── */

static const char *make_relative(const char *path)
{
    if (path == NULL) return "";
    while (*path == '/') path++;
    return path;
}

/* ── TLV encoding helpers ───────────────────────────────────────────────── */

static bool put_entry(uint8_t *buf, size_t cap, size_t *idx,
                      uint8_t type, uint32_t size, const char *name)
{
    size_t nameLen = strlen(name);
    if (nameLen > 120) nameLen = 120;
    size_t entryLen = 1 + 4 + 1 + nameLen;
    if (*idx + 2 + entryLen > cap) return false;

    buf[(*idx)++] = T_FILE_ENTRY;
    buf[(*idx)++] = (uint8_t)entryLen;
    buf[(*idx)++] = type;
    buf[(*idx)++] = (uint8_t)(size & 0xFF);
    buf[(*idx)++] = (uint8_t)((size >> 8) & 0xFF);
    buf[(*idx)++] = (uint8_t)((size >> 16) & 0xFF);
    buf[(*idx)++] = (uint8_t)((size >> 24) & 0xFF);
    buf[(*idx)++] = (uint8_t)nameLen;
    memcpy(&buf[*idx], name, nameLen);
    *idx += nameLen;
    return true;
}

static bool put_status(uint8_t *buf, size_t cap, size_t *idx,
                       uint8_t code, const char *msg)
{
    size_t msgLen = (msg != NULL) ? strlen(msg) : 0;
    if (msgLen > 60) msgLen = 60;
    size_t payLen = 1 + msgLen;
    if (*idx + 2 + payLen > cap) return false;

    buf[(*idx)++] = T_FILE_STATUS;
    buf[(*idx)++] = (uint8_t)payLen;
    buf[(*idx)++] = code;
    if (msgLen > 0) {
        memcpy(&buf[*idx], msg, msgLen);
        *idx += msgLen;
    }
    return true;
}

static void send_response(uint8_t *buf, size_t len)
{
    ESP32_SendFrame(buf, len);
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Post (called from Esp_HandleFrame — must be fast, non-blocking)
 * ══════════════════════════════════════════════════════════════════════════ */

bool FileMgr_Post(uint8_t op, const uint8_t *data, size_t len)
{
    if (s_pending) return false;  /* busy with previous request */

    s_op = op;
    memset(s_path1, 0, sizeof(s_path1));
    memset(s_path2, 0, sizeof(s_path2));
    s_data_len = 0;

    if (op == T_FILE_RENAME && data != NULL && len >= 3) {
        /* Rename format: [oldLen(1)] [oldPath(N)] [newPath(M)] */
        uint8_t oldLen = data[0];
        if ((size_t)(oldLen + 1) <= len) {
            size_t ol = (oldLen < FM_PATH_MAX - 1) ? oldLen : FM_PATH_MAX - 1;
            memcpy(s_path1, &data[1], ol);
            size_t nl = len - 1 - oldLen;
            if (nl > FM_PATH_MAX - 1) nl = FM_PATH_MAX - 1;
            memcpy(s_path2, &data[1 + oldLen], nl);
        }
    } else if (data != NULL && len > 0) {
        size_t copyLen = (len < FM_PATH_MAX - 1) ? len : FM_PATH_MAX - 1;
        memcpy(s_path1, data, copyLen);
    }

    s_data_len = len;
    s_pending = true;
    return true;
}

bool FileMgr_HasPending(void)
{
    return s_pending;
}

/* ══════════════════════════════════════════════════════════════════════════
 *  Process (called from SdCardThread — can use SYS_FS safely)
 * ══════════════════════════════════════════════════════════════════════════ */

void FileMgr_Process(void)
{
    if (!s_pending) return;

    /* Build response header: reply opcode + status placeholder */
    size_t ri = 0;
    s_rsp[ri++] = 0x81;    /* reply to GET (0x01 | 0x80) */
    s_rsp[ri++] = 0x00;    /* status OK placeholder */

    if (!SD_Service_IsMounted()) {
        s_rsp[1] = 0x04;   /* apply fail */
        put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_NOT_MOUNTED, "SD not mounted");
        send_response(s_rsp, ri);
        s_pending = false;
        return;
    }

    /* Ensure current drive is set */
    SYS_FS_CurrentDriveSet(FM_MOUNT_NAME);

    switch (s_op) {

    /* ── List directory ─────────────────────────────────────────────── */
    case T_FILE_LIST:
    {
        const char *relPath = make_relative(s_path1);

        BlockingUART3_WriteString33("FM: raw path1='" );
        BlockingUART3_WriteString33(s_path1);
        BlockingUART3_WriteString33("' rel='");
        BlockingUART3_WriteString33(relPath[0] ? relPath : "(root)");
        BlockingUART3_WriteString33("'\r\n");

        SYS_FS_HANDLE dirH;
        if (relPath[0] == '\0') {
            dirH = SYS_FS_DirOpen(FM_MOUNT_NAME);
        } else {
            dirH = SYS_FS_DirOpen(relPath);
        }

        if (dirH == SYS_FS_HANDLE_INVALID) {
            BlockingUART3_WriteString33("FM: DirOpen FAIL\r\n");
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_NOT_FOUND, "Dir not found");
            break;
        }

        BlockingUART3_WriteString33("FM: DirOpen OK\r\n");

        /* Use static stat to avoid stack overflow on PIC32MM.
         * SYS_FS_FSTAT with LFN can be 300+ bytes. */
        static SYS_FS_FSTAT stat;
        int count = 0;

        /* Clear stat before use */
        memset(&stat, 0, sizeof(stat));

        while (count < 50 && SYS_FS_DirRead(dirH, &stat) == SYS_FS_RES_SUCCESS) {
            if (stat.fname[0] == '\0') break;
            if (stat.fname[0] == '.') continue;

            uint8_t type = (stat.fattrib & SYS_FS_ATTR_DIR) ? FENTRY_DIR : FENTRY_FILE;

            BlockingUART3_WriteString33("FM:  -> ");
            BlockingUART3_WriteString33(stat.fname);
            BlockingUART3_WriteString33("\r\n");

            if (!put_entry(s_rsp, FM_RSP_MAX, &ri, type, (uint32_t)stat.fsize, stat.fname)) {
                break;
            }
            count++;
        }
        SYS_FS_DirClose(dirH);

        char dbg[32];
        snprintf(dbg, sizeof(dbg), "FM: %d entries\r\n", count);
        BlockingUART3_WriteString33(dbg);

        put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "OK");
        break;
    }

    /* ── Delete ─────────────────────────────────────────────────────── */
    case T_FILE_DELETE:
    {
        const char *relPath = make_relative(s_path1);
        if (relPath[0] == '\0') {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_BAD_PARAM, "Empty path");
            break;
        }
        BlockingUART3_WriteString33("FM: delete '");
        BlockingUART3_WriteString33(relPath);
        BlockingUART3_WriteString33("'\r\n");

        if (SYS_FS_FileDirectoryRemove(relPath) == SYS_FS_RES_SUCCESS) {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "Deleted");
        } else {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_IO_ERROR, "Delete failed");
        }
        s_rsp[0] = 0x82;  /* reply to SET */
        break;
    }

    /* ── Rename ─────────────────────────────────────────────────────── */
    case T_FILE_RENAME:
    {
        const char *relOld = make_relative(s_path1);
        const char *relNew = make_relative(s_path2);
        if (relOld[0] == '\0' || relNew[0] == '\0') {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_BAD_PARAM, "Bad paths");
            break;
        }
        BlockingUART3_WriteString33("FM: rename '");
        BlockingUART3_WriteString33(relOld);
        BlockingUART3_WriteString33("' -> '");
        BlockingUART3_WriteString33(relNew);
        BlockingUART3_WriteString33("'\r\n");

        if (SYS_FS_FileDirectoryRenameMove(relOld, relNew) == SYS_FS_RES_SUCCESS) {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "Renamed");
        } else {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_IO_ERROR, "Rename failed");
        }
        s_rsp[0] = 0x82;
        break;
    }

    /* ── Mkdir ──────────────────────────────────────────────────────── */
    case T_FILE_MKDIR:
    {
        const char *relPath = make_relative(s_path1);
        if (relPath[0] == '\0') {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_BAD_PARAM, "Empty path");
            break;
        }
        BlockingUART3_WriteString33("FM: mkdir '");
        BlockingUART3_WriteString33(relPath);
        BlockingUART3_WriteString33("'\r\n");

        if (SYS_FS_DirectoryMake(relPath) == SYS_FS_RES_SUCCESS) {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "Created");
        } else {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_IO_ERROR, "Mkdir failed");
        }
        s_rsp[0] = 0x82;
        break;
    }

    /* ── Create file ────────────────────────────────────────────────── */
    case T_FILE_CREATE:
    {
        const char *relPath = make_relative(s_path1);
        if (relPath[0] == '\0') {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_BAD_PARAM, "Empty path");
            break;
        }
        BlockingUART3_WriteString33("FM: create '");
        BlockingUART3_WriteString33(relPath);
        BlockingUART3_WriteString33("'\r\n");

        SYS_FS_HANDLE fh = SYS_FS_FileOpen(relPath, SYS_FS_FILE_OPEN_WRITE);
        if (fh != SYS_FS_HANDLE_INVALID) {
            SYS_FS_FileClose(fh);
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "Created");
        } else {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_IO_ERROR, "Create failed");
        }
        s_rsp[0] = 0x82;
        break;
    }

    /* ── Get info ───────────────────────────────────────────────────── */
    case T_FILE_INFO:
    {
        const char *relPath = make_relative(s_path1);
        static SYS_FS_FSTAT stat;
        memset(&stat, 0, sizeof(stat));
        if (SYS_FS_FileStat(relPath, &stat) == SYS_FS_RES_SUCCESS) {
            uint8_t type = (stat.fattrib & SYS_FS_ATTR_DIR) ? FENTRY_DIR : FENTRY_FILE;
            put_entry(s_rsp, FM_RSP_MAX, &ri, type, (uint32_t)stat.fsize, stat.fname);
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "OK");
        } else {
            put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_NOT_FOUND, "Not found");
        }
        break;
    }

    /* ── Storage info (total / free / used in KB) ─────────────────────── */
    case T_FILE_STORAGE:
    {
        BlockingUART3_WriteString33("FM: storage query\r\n");

        uint32_t totalKB = 0, freeKB = 0;
        uint32_t totalClusters = 0, freeClusters = 0;
        uint32_t sectorPerCluster = 0, bytesPerSector = 0;

        /* Use SYS_FS_DriveSectorGet to get cluster/sector info */
        if (SYS_FS_DrivePartition(FM_MOUNT_NAME, NULL, NULL) == SYS_FS_RES_FAILURE) {
            /* fallback: can't query */
        }

        /* Get total and free clusters via FatFS f_getfree */
        /* SYS_FS doesn't expose f_getfree directly, but we can
           use SYS_FS_DriveSectorGet for sector size */

        /* Harmony 3 approach: use the underlying FatFS directly */
        {
            FATFS *fs = NULL;
            DWORD fre_clust;
            FRESULT fres = f_getfree("0:", &fre_clust, &fs);
            if (fres == FR_OK && fs != NULL) {
                /* Calculate sizes in KB */
                uint32_t secPerClust = fs->csize;
                uint32_t totalSectors = (fs->n_fatent - 2) * secPerClust;
                uint32_t freeSectors  = fre_clust * secPerClust;
                /* Sectors are 512 bytes */
                totalKB = totalSectors / 2;
                freeKB  = freeSectors / 2;
            } else {
                BlockingUART3_WriteString33("FM: f_getfree FAIL\r\n");
            }
        }

        uint32_t usedKB = (totalKB > freeKB) ? (totalKB - freeKB) : 0;

        char dbg[64];
        snprintf(dbg, sizeof(dbg), "FM: total=%luKB free=%luKB used=%luKB\r\n",
                 (unsigned long)totalKB, (unsigned long)freeKB, (unsigned long)usedKB);
        BlockingUART3_WriteString33(dbg);

        /* Response: T_FILE_STORAGE len=12 [totalKB(4) freeKB(4) usedKB(4)] */
        if (ri + 14 <= FM_RSP_MAX) {
            s_rsp[ri++] = T_FILE_STORAGE;
            s_rsp[ri++] = 12;
            s_rsp[ri++] = (uint8_t)(totalKB & 0xFF);
            s_rsp[ri++] = (uint8_t)((totalKB >> 8) & 0xFF);
            s_rsp[ri++] = (uint8_t)((totalKB >> 16) & 0xFF);
            s_rsp[ri++] = (uint8_t)((totalKB >> 24) & 0xFF);
            s_rsp[ri++] = (uint8_t)(freeKB & 0xFF);
            s_rsp[ri++] = (uint8_t)((freeKB >> 8) & 0xFF);
            s_rsp[ri++] = (uint8_t)((freeKB >> 16) & 0xFF);
            s_rsp[ri++] = (uint8_t)((freeKB >> 24) & 0xFF);
            s_rsp[ri++] = (uint8_t)(usedKB & 0xFF);
            s_rsp[ri++] = (uint8_t)((usedKB >> 8) & 0xFF);
            s_rsp[ri++] = (uint8_t)((usedKB >> 16) & 0xFF);
            s_rsp[ri++] = (uint8_t)((usedKB >> 24) & 0xFF);
        }
        put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_OK, "OK");
        break;
    }

    default:
        put_status(s_rsp, FM_RSP_MAX, &ri, FMGR_BAD_PARAM, "Unknown op");
        break;
    }

    /* Send the response */
    send_response(s_rsp, ri);
    s_pending = false;
}
