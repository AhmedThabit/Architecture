#include <stdint.h>
#include "common/schema.h"
#include "hal/io_monitor/io_monitor.h"

uint8_t IO_GetInputsMask(void){ // read your 4 inputs, normalise NO/NC
  uint8_t raw = 0/* read GPIOs */;
  uint8_t inv = g_device_cfg.io.in_no_nc; // 1 bit means NC -> invert
  return (raw ^ inv) & 0x0F;
}
uint8_t IO_GetOutputsMask(void){ return /* read 4 outputs bits */ 0 & 0x0F; }
uint16_t IO_GetMoistPct10(void){
  uint16_t adc = 5/* read ADC */;
  // map to 0..1000; adjust calibration as needed
  return (uint16_t)((adc * 1000UL)/4095UL);
}
