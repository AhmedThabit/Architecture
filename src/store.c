#include "flash_w25q32.h"
#include "schema.h"
#include <string.h>
// TODO: implement flash read/write; simple header {magic,version,len,crc}

#define CFG_FLASH_ADDR    0x00000000u
#define CFG_MAGIC         0x43464731u  // 'CFG1' in ASCII

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
    Flash_Init();

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

const char* phonebook_get_number(uint8_t slot) {
    return (slot < 16 && g_cfg.phonebook.numbers[slot][0] != '\0')
           ? g_cfg.phonebook.numbers[slot]
           : NULL;
}

void phonebook_set_number(uint8_t slot, const char* num) {
    if (slot < 16 && num) {
        strncpy(g_cfg.phonebook.numbers[slot], num, 23);
        g_cfg.phonebook.numbers[slot][23] = '\0';
        Cfg_Save();    // persist to flash
    }
}

void phonebook_delete_number(uint8_t slot) {
    if (slot < 16) {
        g_cfg.phonebook.numbers[slot][0] = '\0';
        Cfg_Save();
    }
}

void phonebook_set_default(uint8_t slot) {
    if (slot < 16) {
        g_cfg.phonebook.default_index = slot;
        Cfg_Save();
    }
}
uint8_t phonebook_get_default(void) { return g_cfg.phonebook.default_index; }
