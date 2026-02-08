#include "flash_w25q32.h"
#include "schema.h"
#include <string.h>
// TODO: implement flash read/write; simple header {magic,version,len,crc}
// store.c (or wherever these live)
#include "phonebook_flash.h"

#define CFG_FLASH_ADDR    0x00000000u
#define CFG_MAGIC         0x43464731u  // 'CFG1' in ASCII

void UART3_WriteString33(const char *str); // forward declaration

static void uart3_write_hex(uint8_t byte) {
    const char hex[] = "0123456789ABCDEF";
    char out[3] = {hex[byte >> 4], hex[byte & 0x0F], 0};
    UART3_WriteString33(out);
}


typedef struct {
    uint32_t magic;
    uint32_t length;
    uint32_t version;
    uint32_t crc;
} CfgHeader;

DeviceCfg g_cfg;

void Cfg_Defaults(void){
  memset(&g_cfg,0,sizeof(g_cfg));
  g_cfg.version = 0x00010100; // v1.1.0
}

static void Cfg_WriteHeaderAndData(const CfgHeader *hdr, const DeviceCfg *cfg)
{
    uint8_t buf[sizeof(CfgHeader) + sizeof(DeviceCfg)];
    memcpy(buf, hdr, sizeof(CfgHeader));
    memcpy(buf + sizeof(CfgHeader), cfg, sizeof(DeviceCfg));

    Flash_EraseSector(CFG_FLASH_ADDR);
    Flash_Write(CFG_FLASH_ADDR, buf, sizeof(buf));
}

void Cfg_Load(void)
{
    //Flash_Init();
//test comment
    /* Initialise g_cfg to defaults.  This sets g_cfg.version to the current
     * firmware version (0x00010100) before we validate the header.  Without
     * this, g_cfg.version is zero on power?up and the version check fails.   */
    Cfg_Defaults();

    CfgHeader hdr;
    Flash_Read(CFG_FLASH_ADDR, (uint8_t*)&hdr, sizeof(hdr));

    /* Reject the stored config if the magic, length or version don?t match.   */
    if (hdr.magic != CFG_MAGIC ||
        hdr.length != sizeof(DeviceCfg) ||
        hdr.version != g_cfg.version) {
        return;  // leave defaults
    }

    /* Load the config from flash and verify its CRC.  If the CRC fails,
     * fall back to defaults. */
    Flash_Read(CFG_FLASH_ADDR + sizeof(CfgHeader),
               (uint8_t*)&g_cfg, sizeof(g_cfg));
    uint32_t calc = Flash_CRC32(&g_cfg, sizeof(g_cfg));
    if (calc != hdr.crc) {
        Cfg_Defaults();
    }
}


void Cfg_Save(void)
{
    CfgHeader hdr;
    hdr.magic   = CFG_MAGIC;
    hdr.length  = sizeof(DeviceCfg);
    hdr.version = g_cfg.version;
    hdr.crc     = Flash_CRC32(&g_cfg, sizeof(g_cfg));
    Cfg_WriteHeaderAndData(&hdr, &g_cfg);
}

int Cfg_Validate(void)
{
    // reload and recompute CRC; return 0 if ok, non?zero if corrupted
    CfgHeader hdr;
    Flash_Read(CFG_FLASH_ADDR, (uint8_t*)&hdr, sizeof(hdr));
    if (hdr.magic != CFG_MAGIC) return -1;
    DeviceCfg tmp;
    Flash_Read(CFG_FLASH_ADDR + sizeof(CfgHeader), (uint8_t*)&tmp, sizeof(tmp));
    return (Flash_CRC32(&tmp, sizeof(tmp)) == hdr.crc) ? 0 : -1;
}
//
//const char* phonebook_get_number(uint8_t slot) {
//    return (slot < 16 && g_cfg.phonebook.numbers[slot][0] != '\0')
//           ? g_cfg.phonebook.numbers[slot]
//           : NULL;
//}
//
//void phonebook_set_number(uint8_t slot, const char* num) {
//    if (slot < 16 && num) {
//        strncpy(g_cfg.phonebook.numbers[slot], num, 23);
//        g_cfg.phonebook.numbers[slot][23] = '\0';
//        Cfg_Save();    // persist to flash
//    }
//}
//
//void phonebook_delete_number(uint8_t slot) {
//    if (slot < 16) {
//        g_cfg.phonebook.numbers[slot][0] = '\0';
//        Cfg_Save();
//    }
//}
//
//void phonebook_set_default(uint8_t slot) {
//    if (slot < 16) {
//        g_cfg.phonebook.default_index = slot;
//        Cfg_Save();
//    }
//}
//uint8_t phonebook_get_default(void) {
//    return g_cfg.phonebook.default_index;
//}


#include "phonebook_flash.h"   // add this include at top

// NOTE: PB_MAX_LEN is defined in phonebook_flash.h

// We use phonebook_flash.c as the backend. This avoids g_cfg layout issues.

// Small static buffer for one number; safe because send_phonebook_list()
// copies the string immediately into its reply buffer.
// store.c

// Return pointer to a static buffer containing the number, or NULL if empty.
const char* phonebook_get_number(uint8_t slot) {
    static char buf[PB_MAX_LEN];

    UART3_WriteString33("phonebook_get_number(slot=");
    uart3_write_hex(slot);
    UART3_WriteString33(") [flash]\r\n");

    if (slot >= PB_SLOT_COUNT) {
        UART3_WriteString33(" -> invalid slot, returns NULL\r\n");
        return NULL;
    }

    if (!PhonebookFlash_GetNumber(slot, buf, sizeof(buf))) {
        UART3_WriteString33(" -> PhonebookFlash_GetNumber FAILED, returns NULL\r\n");
        return NULL;
    }

    if (buf[0] == '\0') {
        UART3_WriteString33(" -> empty string, returns NULL\r\n");
        return NULL;
    }

    UART3_WriteString33(" -> returns \"");
    for (int i = 0; buf[i] && i < PB_MAX_LEN; ++i) {
        UART3_WriteString33((char[]){buf[i], 0});
    }
    UART3_WriteString33("\"\r\n");

    return buf;
}

void phonebook_set_number(uint8_t slot, const char* num) {
    UART3_WriteString33("phonebook_set_number(slot=");
    uart3_write_hex(slot);
    UART3_WriteString33(", num=\"");
    if (num) {
        for (const char* p = num; *p; ++p) {
            UART3_WriteString33((char[]){*p, 0});
        }
    }
    UART3_WriteString33("\") [flash]\r\n");

    if (!PhonebookFlash_SetNumber(slot, num)) {
        UART3_WriteString33(" PhonebookFlash_SetNumber FAILED\r\n");
    } else {
        UART3_WriteString33(" PhonebookFlash_SetNumber OK\r\n");
    }
}

void phonebook_set_default(uint8_t slot) {
    UART3_WriteString33("phonebook_set_default(slot=");
    uart3_write_hex(slot);
    UART3_WriteString33(") [flash]\r\n");

    if (!PhonebookFlash_SetDefault(slot)) {
        UART3_WriteString33(" PhonebookFlash_SetDefault FAILED\r\n");
    } else {
        UART3_WriteString33(" PhonebookFlash_SetDefault OK\r\n");
    }
}

uint8_t phonebook_get_default(void) {
    uint8_t def = PhonebookFlash_GetDefault();
    UART3_WriteString33("phonebook_get_default() [flash] -> ");
    uart3_write_hex(def);
    UART3_WriteString33("\r\n");
    return def;
}



// Small static buffer for one number; safe because send_phonebook_list()
// copies the string immediately into its reply buffer.

//const char* phonebook_get_number(uint8_t slot) {
//    UART3_WriteString33("phonebook_get_number(slot=");
//    uart3_write_hex(slot);
//    UART3_WriteString33(")\r\n");
//
//    if (slot < 16 && g_cfg.phonebook.numbers[slot][0] != '\0') {
//        UART3_WriteString33(" -> returns \"");
//        for (int i = 0; i < 23 && g_cfg.phonebook.numbers[slot][i]; ++i) {
//            UART3_WriteString33((char[]){g_cfg.phonebook.numbers[slot][i], 0});
//        }
//        UART3_WriteString33("\"\r\n");
//        return g_cfg.phonebook.numbers[slot];
//    } else {
//        UART3_WriteString33(" -> returns NULL (empty)\r\n");
//        return NULL;
//    }
//}



//const char* phonebook_get_number(uint8_t slot) {
//    static char buf[PB_MAX_LEN];
//
//    if (PhonebookFlash_GetNumber(slot, buf, sizeof(buf))) {
//        if (buf[0] != '\0') {
//            return buf;
//        }
//    }
//    return NULL;
//}

//void phonebook_set_number(uint8_t slot, const char* num) {
//    
//    
//    
//    
//    // This uses the same Flash_Erase/Write/CRC mechanism as your working test
//    PhonebookFlash_SetNumber(slot, num);
//}

//void phonebook_set_number(uint8_t slot, const char* num) {
//    UART3_WriteString33("phonebook_set_number(slot=");
//    uart3_write_hex(slot);
//    UART3_WriteString33(", num=\"");
//    if (num) {
//        for (const char* p = num; *p; ++p) {
//            UART3_WriteString33((char[]){*p, 0});
//        }
//    }
//    UART3_WriteString33("\")\r\n");
//
//    if (slot < 16 && num) {
//        strncpy(g_cfg.phonebook.numbers[slot], num, 23);
//        g_cfg.phonebook.numbers[slot][23] = '\0';
//
//        UART3_WriteString33("g_cfg.phonebook.numbers[slot] now=\"");
//        for (int i = 0; i < 23 && g_cfg.phonebook.numbers[slot][i]; ++i) {
//            UART3_WriteString33((char[]){g_cfg.phonebook.numbers[slot][i], 0});
//        }
//        UART3_WriteString33("\"\r\n");
//
//        Cfg_Save();    // persist to flash
//        UART3_WriteString33("Cfg_Save() called\r\n");
//    } else {
//        UART3_WriteString33("phonebook_set_number: invalid slot or NULL num\r\n");
//    }
//}


//
//void phonebook_delete_number(uint8_t slot) {
//    //PhonebookFlash_ClearNumber(slot);   // if you don?t have this, we can add it
//}

//void phonebook_set_default(uint8_t slot) {
//    PhonebookFlash_SetDefault(slot);
//}

//void phonebook_set_default(uint8_t slot) {
//    UART3_WriteString33("phonebook_set_default(slot=");
//    uart3_write_hex(slot);
//    UART3_WriteString33(")\r\n");
//
//    if (slot < 16) {
//        g_cfg.phonebook.default_index = slot;
//        Cfg_Save();
//        UART3_WriteString33("default_index updated and Cfg_Save() called\r\n");
//    } else {
//        UART3_WriteString33("phonebook_set_default: invalid slot\r\n");
//    }
//}
//
//uint8_t phonebook_get_default(void) {
//    UART3_WriteString33("phonebook_get_default() -> ");
//    uart3_write_hex(g_cfg.phonebook.default_index);
//    UART3_WriteString33("\r\n");
//    return g_cfg.phonebook.default_index;
//}



//uint8_t phonebook_get_default(void) {
//    return PhonebookFlash_GetDefault();
//}


//void phonebook_set_default(uint8_t slot) {
//    UART3_WriteString33("phonebook_set_default(slot=");
//    uart3_write_hex(slot);
//    UART3_WriteString33(")\r\n");
//
//    if (slot < 16) {
//        g_cfg.phonebook.default_index = slot;
//        Cfg_Save();
//        UART3_WriteString33("default_index updated and Cfg_Save() called\r\n");
//    } else {
//        UART3_WriteString33("phonebook_set_default: invalid slot\r\n");
//    }
//}

//uint8_t phonebook_get_default(void) {
//    UART3_WriteString33("phonebook_get_default() -> ");
//    uart3_write_hex(g_cfg.phonebook.default_index);
//    UART3_WriteString33("\r\n");
//    return g_cfg.phonebook.default_index;
//}

