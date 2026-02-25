#include "config/default/peripheral/uart/plib_uart3.h"
#include "gsm_le910.h"
#include <stdio.h>
#include <string.h>

static int at_sendln(const char* s){ /*uart3_write((const uint8_t*)s, strlen(s)); uart3_write((uint8_t*)"\r",1); */ return 0; }
static int at_expect_ok(uint32_t ms){ /* read lines until OK/ERROR with timeout */ return 0; }

int GSM_Init(void){
  at_sendln("ATE0");          at_expect_ok(500);
  at_sendln("AT+CMEE=2");     at_expect_ok(500);
  at_sendln("AT+CFUN=1");     at_expect_ok(2000);
  // wait registered (poll +CEREG? 1/5)
  at_sendln("AT+CSCS=\"GSM\"");  at_expect_ok(500);
  at_sendln("AT+CMGF=1");        at_expect_ok(500); // SMS TEXT mode (27.005) 
  at_sendln("AT+CNMI=2,1,0,0,0"); at_expect_ok(500); // URC on new SMS 
  at_sendln("AT+CPMS=\"ME\",\"ME\",\"ME\""); at_expect_ok(500);
  return 0;
}

int GSM_SendSMS(const char* num, const char* text){
  char buf[64]; snprintf(buf,sizeof(buf),"AT+CMGS=\"%s\"", num);
  at_sendln(buf); /* wait for '>' prompt */
  //uart3_write((const uint8_t*)text, strlen(text));
  //uint8_t ctrlz = 0x1A; uart3_write(&ctrlz,1);
  // expect +CMGS and OK per 27.005 AT+CMGS 
  return 0;
}

// Called when we get +CMTI: "ME",<idx>
int GSM_ReadSMS(int idx, char* from, size_t from_sz, char* body, size_t body_sz){
  char c[32]; snprintf(c,sizeof(c),"AT+CMGR=%d",idx);
  at_sendln(c);
  // parse +CMGR header and body lines; copy to from/body
  return 1; // 1=ok
}
int GSM_DeleteSMS(int idx){ char c[24]; snprintf(c,sizeof(c),"AT+CMGD=%d",idx); at_sendln(c); return 0; }
int GSM_GetRSSI(void){ at_sendln("AT+CSQ"); /* parse */ return -70; }
