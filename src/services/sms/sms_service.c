#include "services/gsm/gsm_le910.h"
#include "common/schema.h"
#include "hal/io_monitor/io_monitor.h"

#include <string.h>
#include <stdio.h>

static int is_in_phonebook(const char* from){
  for(int i=0;i<16;i++) if (g_device_cfg.phonebook.numbers[i][0] &&
      strcmp(from, g_device_cfg.phonebook.numbers[i])==0) return 1;
  return 0;
}
static void compose_status(char* out, size_t n){
  uint8_t in = IO_GetInputsMask(), outm = IO_GetOutputsMask();
  uint16_t m10 = IO_GetMoistPct10();
  int rssi = GSM_GetRSSI();
  uint8_t alarm = AlarmMgr_GetCode();
  snprintf(out,n,"IN:%u%u%u%u; OUT:%u%u%u%u; MOIST:%u.%u%%; ALARM:%u; RSSI:%ddBm",
    !!(in&1),!!(in&2),!!(in&4),!!(in&8),
    !!(outm&1),!!(outm&2),!!(outm&4),!!(outm&8),
    m10/10, m10%10, alarm, rssi);
}

// Call on +CMTI -> read idx
void SMS_OnNewMessage(int idx){
  char from[32], text[192];
  if (!GSM_ReadSMS(idx, from,sizeof(from), text,sizeof(text))) return;

  // very simple parser: STATUS [PIN], RESET [PIN], NUM SET <n> <+20...>
  if (strncasecmp(text,"STATUS",6)==0){
    char body[200]; compose_status(body,sizeof(body));
    GSM_SendSMS(from, body);
  } else if (strncasecmp(text,"RESET",5)==0){
    AlarmMgr_Reset();
    GSM_SendSMS(from, "OK: Alarm reset");
  } else if (strncasecmp(text,"NUM",3)==0){
    // NUM SET 3 +201234567890
    int slot=0; char cmd[6], sub[6], num[32];
    if (sscanf(text,"%5s %5s %d %31s",cmd,sub,&slot,num)==4 && slot>=1 && slot<=16){
      strncpy(g_device_cfg.phonebook.numbers[slot-1], num, 23);
      g_device_cfg.phonebook.numbers[slot-1][23]=0;
      Cfg_Save();
      GSM_SendSMS(from,"OK: number saved");
    }
  }
  GSM_DeleteSMS(idx);
}
