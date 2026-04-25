/**
 * @file    app_config.h
 * @brief   Centralized application configuration for the Emergency Audio Dialer.
 *
 * @details All hardware mappings, peripheral assignments, buffer sizes, and
 *          feature flags live here.  When you change hardware (swap modem,
 *          rewire a UART, add a peripheral) you edit ONE file.
 *
 *          Naming convention used throughout the project:
 *            Macros  : CFG_MODULE_PARAMETER
 *            Types   : ModuleName_TypeName (PascalCase)
 *            Functions: Module_Action()    (PascalCase prefix + verb)
 *            Locals  : snake_case
 *            Statics : s_snake_case
 *            Globals : g_snake_case  (minimised -- prefer accessor functions)
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "services/modem_api/modem_api_types.h"   /* For audio enum types    */

/* =========================================================================
 *  DEBUG -- toggle UART3 debug output
 * =========================================================================
 *  Set to 1 when modem is DISCONNECTED and UART3 is wired to a serial
 *  terminal for debugging.  Set to 0 for production (modem connected).
 */
#define CFG_DBG_UART3       0       /* 1 = UART3 debug ON, 0 = OFF */

/* ===========================================================================
 *  MODEM -- Telit LE910C4-EU  (4G Cat-4, Linux firmware, USB Audio)
 * =========================================================================== */

/** Which UART peripheral the modem is wired to (PLIB prefix).               */
#define CFG_MODEM_UART_WRITE        UART3_Write
#define CFG_MODEM_UART_READ         UART3_Read
#define CFG_MODEM_UART_READ_COUNT   UART3_ReadCountGet
#define CFG_MODEM_UART_TX_COMPLETE  UART3_TransmitComplete
#define CFG_MODEM_UART_INIT         UART3_Initialize
#define CFG_MODEM_UART_CB_REG       UART3_ReadCallbackRegister
#define CFG_MODEM_UART_THRESH_SET   UART3_ReadThresholdSet
#define CFG_MODEM_UART_NOTIFY_EN    UART3_ReadNotificationEnable

/** Modem UART baud rate (must match hardware / MCC config).                 */
#define CFG_MODEM_BAUD              115200u

/** Modem type selection -- uncomment exactly ONE.
 *  The GSM service layer uses this to select the correct AT command set.     */
#define CFG_MODEM_TELIT_LE910C4     1
// #define CFG_MODEM_TELIT_FN990A28 1   /* Future: 5G Sub-6 GHz                */
// #define CFG_MODEM_QUECTEL_EG25   1   /* Alternative                         */

/** AT-engine buffer sizes                                                    */
#define CFG_MODEM_RX_LINE_MAX       192u    /**< Max chars in one AT line     */
#define CFG_MODEM_RX_LINES          8u      /**< Ring-buffer depth (lines)    */
#define CFG_MODEM_RX_RAW_BUF       512u     /**< Raw RX accumulator           */

/** AT-engine timeouts (milliseconds)                                        */
#define CFG_MODEM_AT_TIMEOUT_MS     1500u   /**< Default per-command timeout  */
#define CFG_MODEM_BOOT_TIMEOUT_MS   60000u  /**< Wait for network after boot  */
#define CFG_MODEM_SMS_SEND_TIMEOUT  30000u  /**< Wait for +CMGS / OK          */
#define CFG_MODEM_SMS_READ_TIMEOUT  2000u

/* ===========================================================================
 *  AUDIO CODEC -- MAX9867ETJ+ (controlled by modem via I2C + Aux-PCM)
 * ===========================================================================
 *
 *  Hardware wiring:
 *    LE910C4 Aux-PCM  ->  MAX9867 PCM interface (BCLK, LRCLK, SDIN, SDOUT)
 *    LE910C4 I2C      ->  MAX9867 I2C interface (SCL, SDA, addr 0x18)
 *    MAX9867 MCLK     <-  Either modem PCM_CLK or external crystal
 *    MAX9867 MIC      <-  Electret/MEMS microphone (differential input)
 *    MAX9867 HP OUT   ->  Speaker / earpiece (differential output)
 *
 *  The PIC32MM has NO direct connection to the MAX9867.
 *  All codec control is through modem AT commands over UART3.
 * =========================================================================== */

/** --- Clock Source Selection ---------------------------------------------
 *
 *  MODEM_CODEC_CLK_MODEM:
 *    - MCLK driven by modem's PCM_CLK output
 *    - Simpler hardware -- no crystal needed
 *    - Clock only active when modem audio subsystem is running
 *
 *  MODEM_CODEC_CLK_CRYSTAL:
 *    - MCLK driven by an external crystal oscillator
 *    - MAX9867 always has a clock, even when modem audio is idle
 *    - Change CFG_AUDIO_CRYSTAL_FREQ_HZ to match your oscillator
 */
#define CFG_AUDIO_CLK_SOURCE        MODEM_CODEC_CLK_MODEM
/* #define CFG_AUDIO_CLK_SOURCE     MODEM_CODEC_CLK_CRYSTAL */

/** External crystal frequency (only used when CLK_SOURCE = CRYSTAL).
 *    12288000  -> 12.288 MHz (exact integer for 8/16/24/32/48 kHz)
 *    13000000  -> 13 MHz     (Telit default, exact for 8/16 kHz)
 *    19200000  -> 19.2 MHz   (common in GSM platforms)                      */
#define CFG_AUDIO_CRYSTAL_FREQ_HZ   13000000u

/** --- Audio Profile Defaults -------------------------------------------- */
#define CFG_AUDIO_DEFAULT_PATH      MODEM_AUDIO_PATH_HANDSET
#define CFG_AUDIO_DEFAULT_PROFILE   MODEM_AUDIO_PROFILE_HANDSFREE
#define CFG_AUDIO_DEFAULT_SPK_VOL   70u     /**< 0-100, mapped to 0-5       */
#define CFG_AUDIO_DEFAULT_MIC_GAIN  60u     /**< 0-100, mapped to 0-7       */

/** --- Voice Processing Defaults ----------------------------------------- */
#define CFG_AUDIO_ECHO_CANCEL       true    /**< Echo canceller on/off      */
#define CFG_AUDIO_NOISE_REDUCE      true    /**< Noise reduction on/off     */
#define CFG_AUDIO_AGC_ENABLE        true    /**< Auto gain control on/off   */

/** --- Audio Loopback Test ----------------------------------------------- */
#define CFG_AUDIO_LOOPBACK_TEST     0       /**< 1 = enable loopback at boot*/

/* ===========================================================================
 *  ESP32 -- Co-processor link (binary TLV protocol over UART)
 * =========================================================================== */
#define CFG_ESP32_UART_WRITE        UART1_Write
#define CFG_ESP32_UART_READ         UART1_Read
#define CFG_ESP32_UART_READ_COUNT   UART1_ReadCountGet
#define CFG_ESP32_UART_TX_COMPLETE  UART1_TransmitComplete
#define CFG_ESP32_UART_CB_REG       UART1_ReadCallbackRegister
#define CFG_ESP32_UART_THRESH_SET   UART1_ReadThresholdSet
#define CFG_ESP32_UART_NOTIFY_EN    UART1_ReadNotificationEnable
#define CFG_ESP32_UART_FREQ_GET     UART1_FrequencyGet
#define CFG_ESP32_UART_SERIAL_SETUP UART1_SerialSetup

#define CFG_ESP32_BAUD              115200u
#define CFG_ESP32_ASCII_TEST        0       /**< 1 = RealTerm ASCII debug     */
#define CFG_ESP32_RX_BUF            512u
#define CFG_ESP32_PAYLOAD_MAX       256u
#define CFG_ESP32_IDLE_TIMEOUT_MS   20u

/* ===========================================================================
 *  DIGITAL I/O -- 4 inputs + 4 outputs on main board
 * ===========================================================================
 *
 *  IMPORTANT: Update these pin definitions to match the actual PCB.
 *  These are GPIO_PIN_Rxy values from plib_gpio.h.
 *  The current values are PLACEHOLDERS for the Curiosity board.
 * =========================================================================== */

/* ===========================================================================
 *  UNIVERSAL I/O -- 4 channels, each configurable as input/output/analogue
 * ===========================================================================
 *
 *  These 4 pins are the actual EDAC 8000 PCB I/O channels.
 *  Each can be: digital input (NO/NC), analogue input (1V/10V/20mA),
 *  or digital output -- configured per-channel via the mobile app.
 *
 *  Pin      ADC Channel    Schematic Label
 *  RA13     AN16           ANA1
 *  RA12     AN17           ANA2
 *  RA11     AN18           ANA3
 *  RB15     AN10           ANA4
 * =========================================================================== */

/** Universal I/O pins (GPIO_PIN_Rxy from plib_gpio.h) */
#define CFG_IO1_PIN                 GPIO_PIN_RA13   /**< ANA1 -- AN16       */
#define CFG_IO2_PIN                 GPIO_PIN_RA12   /**< ANA2 -- AN17       */
#define CFG_IO3_PIN                 GPIO_PIN_RA11   /**< ANA3 -- AN18       */
#define CFG_IO4_PIN                 GPIO_PIN_RB15   /**< ANA4 -- AN10       */

/** ADC channel mapping for analogue mode */
#define CFG_IO1_ADC_CH              ADC_INPUT_POSITIVE_AN16
#define CFG_IO2_ADC_CH              ADC_INPUT_POSITIVE_AN17
#define CFG_IO3_ADC_CH              ADC_INPUT_POSITIVE_AN18
#define CFG_IO4_ADC_CH              ADC_INPUT_POSITIVE_AN10

/** Number of universal I/O channels */
#define CFG_IO_COUNT                4u

/** Legacy aliases (code that still references CFG_DIN_COUNT) */
#define CFG_DIN_COUNT               CFG_IO_COUNT
#define CFG_DOUT_COUNT              CFG_IO_COUNT

/** Digital input pins -- legacy aliases for io_monitor.c */
#define CFG_DIN1_PIN                CFG_IO1_PIN
#define CFG_DIN2_PIN                CFG_IO2_PIN
#define CFG_DIN3_PIN                CFG_IO3_PIN
#define CFG_DIN4_PIN                CFG_IO4_PIN

/** Digital output pins -- same physical pins */
#define CFG_DOUT1_PIN               CFG_IO1_PIN
#define CFG_DOUT2_PIN               CFG_IO2_PIN
#define CFG_DOUT3_PIN               CFG_IO3_PIN
#define CFG_DOUT4_PIN               CFG_IO4_PIN

/** Mains power detection pin (HIGH = mains present, pulled-up on PCB).
 *  Per schematic: MAINFAIL signal -> RB14 (pin 2). */
#define CFG_MAINS_DET_PIN           GPIO_PIN_RB14   /**< Mains detect input   */

/** Battery fail detection pin (HIGH = battery OK, pulled-up on PCB).
 *  Per schematic: BATTFAIL signal -> RA14 (pin 59). */
#define CFG_BATTFAIL_PIN            GPIO_PIN_RA14   /**< Battery fail input   */

/** I/O debounce period (ms) */
#define CFG_IO_DEBOUNCE_MS          10u

/* ===========================================================================
 *  ALARM SYSTEM
 * =========================================================================== */

/** Alarm trigger delay -- input must be stable this long before firing. */
#define CFG_ALARM_TRIGGER_DELAY_MS  500u

/** Alarm clear delay -- input must be normal this long before clearing. */
#define CFG_ALARM_CLEAR_DELAY_MS    2000u

/** Maximum call attempts per alarm event before giving up. */
#define CFG_ALARM_MAX_CALL_RETRIES  10u

/** Delay between call attempts (ms). */
#define CFG_ALARM_CALL_RETRY_MS     15000u

/** Ring timeout -- move to next number if no answer (ms). */
#define CFG_ALARM_RING_TIMEOUT_MS   30000u

/** Alarm scan interval (ms). */
#define CFG_ALARM_SCAN_MS           100u

/** Battery low threshold (mV). 12V SLA -> ~10.8V = low. */
#define CFG_BATTERY_LOW_MV          10800u

/** Battery critical threshold (mV). */
#define CFG_BATTERY_CRITICAL_MV     10200u

/* ===========================================================================
 *  SPI BUS -- Shared between SD card and W25Q32 flash
 * =========================================================================== */
#define CFG_SPI_SD_INIT_HZ          400000u
#define CFG_SPI_SD_FAST_HZ          8000000u
#define CFG_SPI_FLASH_HZ            12000000u

/* ===========================================================================
 *  FLASH -- W25Q32 (32 Mbit = 4 MB)
 * =========================================================================== */
#define CFG_FLASH_TOTAL_SIZE        (4UL * 1024UL * 1024UL)
#define CFG_FLASH_SECTOR_SIZE       4096u
#define CFG_FLASH_PAGE_SIZE         256u

/* ===========================================================================
 *  LED -- Status indicator
 * =========================================================================== */
#define CFG_LED_STATUS_BIT          15u     /**< Port A bit for status LED    */

/* ===========================================================================
 *  PHONEBOOK
 * =========================================================================== */
#define CFG_PHONEBOOK_MAX_ENTRIES   16u
#define CFG_PHONEBOOK_NUM_LEN       24u

/* ===========================================================================
 *  SD CARD
 * =========================================================================== */
#define CFG_SD_MOUNT_RETRIES        3u
#define CFG_SD_DRIVE_PATH           "0:"

/* ===========================================================================
 *  SYSTEM TIMER -- 1 ms tick driven by Timer1
 * =========================================================================== */
#define CFG_SYSTICK_PERIOD_MS       1u

/* ===========================================================================
 *  FIRMWARE IDENTIFICATION
 * =========================================================================== */
#define CFG_FW_VERSION_STRING       "PIC32MM-EDAC 2.1"
#define CFG_BOARD_NAME              "pic32mm_usb_curiosity"

#endif /* APP_CONFIG_H */
