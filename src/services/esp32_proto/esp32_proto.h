/**
 * @file    esp32_proto.h
 * @brief   ESP32 ↔ PIC32 TLV protocol handler (service layer).
 */

#ifndef ESP32_PROTO_H
#define ESP32_PROTO_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/** Main frame handler — called by ESP32_Poll() on valid frame receipt. */
void Esp_HandleFrame(const uint8_t *payload, size_t len);

/** Send the complete phonebook list as a TLV response frame. */
void send_phonebook_list(void);

/** SMS enable/disable control (called from TLV SET handler). */
extern volatile bool sms_enabled;
void handle_sms_enable_cmd(uint8_t flag);

#endif /* ESP32_PROTO_H */
