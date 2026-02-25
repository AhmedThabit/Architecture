// phonebook_flash.h
#ifndef PHONEBOOK_FLASH_H
#define PHONEBOOK_FLASH_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PB_SLOT_COUNT     16
#define PB_MAX_DIGITS     23
#define PB_MAX_LEN        (PB_MAX_DIGITS + 1)   // with '\0'

void PhonebookFlash_Init(void);

bool PhonebookFlash_SetNumber(uint8_t slot, const char *num);
bool PhonebookFlash_GetNumber(uint8_t slot, char *out, size_t outSize);

bool PhonebookFlash_SetDefault(uint8_t slot);
uint8_t PhonebookFlash_GetDefault(void);

bool PhonebookFlash_Clear(void);

#endif
