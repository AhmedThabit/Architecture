#ifndef FFCONF_H
#define FFCONF_H

/* Must match FF_DEFINED in ff.h (R0.16 shows 80386) */
#define FFCONF_DEF 80386

/* Harmony-style alignment macro expected by your ff.h */
#ifndef FS_FATFS_WINDOW_ALIGNMENT
#define FS_FATFS_WINDOW_ALIGNMENT 4
#endif

/* --- Basic config --- */
#define FF_FS_READONLY          0   /* 0: Read/Write enabled */
#define FF_FS_MINIMIZE          0
#define FF_USE_MKFS             1
#define FF_USE_FIND             0
#define FF_USE_STRFUNC          0

/* --- LFN off (simple & smaller) --- */
#define FF_USE_LFN              0
#define FF_LFN_UNICODE          0
#define FF_CODE_PAGE            437
#define FF_FS_RPATH             0

/* --- Media / sector sizes --- */
#define FF_VOLUMES              1
#define FF_MULTI_PARTITION      0
#define FF_MIN_SS               512
#define FF_MAX_SS               512
#define FF_LBA64                0
#define FF_FS_EXFAT             0

/* --- Thread safety off --- */
#define FF_FS_LOCK              0
#define FF_FS_REENTRANT         0

#endif
