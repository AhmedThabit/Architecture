// esp32_comm.h
#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*ESP32_FrameHandler)(const uint8_t* payload, size_t len);

void ESP32_UartInit(void);
bool ESP32_TakeRxFlag(void);   // atomically test & clear
void ESP32_RegisterFrameHandler(ESP32_FrameHandler cb);  // app installs handler
void ESP32_Poll(void);                                   // drain RX & parse
bool ESP32_SendFrame(const uint8_t* payload, size_t len);// TX with framing