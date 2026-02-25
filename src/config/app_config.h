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
 *            Globals : g_snake_case  (minimised — prefer accessor functions)
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

/* ═══════════════════════════════════════════════════════════════════════════
 *  MODEM  — Telit LE910C4-EU  (4G Cat-4, Linux firmware, USB Audio)
 * ═══════════════════════════════════════════════════════════════════════════ */

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

/** Modem type selection — uncomment exactly ONE.
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

/* ═══════════════════════════════════════════════════════════════════════════
 *  ESP32  — Co-processor link (binary TLV protocol over UART)
 * ═══════════════════════════════════════════════════════════════════════════ */
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

/* ═══════════════════════════════════════════════════════════════════════════
 *  SPI BUS  — Shared between SD card and W25Q32 flash
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_SPI_SD_INIT_HZ          400000u
#define CFG_SPI_SD_FAST_HZ          8000000u
#define CFG_SPI_FLASH_HZ            12000000u

/* ═══════════════════════════════════════════════════════════════════════════
 *  FLASH  — W25Q32 (32 Mbit = 4 MB)
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_FLASH_TOTAL_SIZE        (4UL * 1024UL * 1024UL)
#define CFG_FLASH_SECTOR_SIZE       4096u
#define CFG_FLASH_PAGE_SIZE         256u

/* ═══════════════════════════════════════════════════════════════════════════
 *  LED  — Status indicator
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_LED_STATUS_BIT          15u     /**< Port A bit for status LED    */

/* ═══════════════════════════════════════════════════════════════════════════
 *  PHONEBOOK
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_PHONEBOOK_MAX_ENTRIES   16u
#define CFG_PHONEBOOK_NUM_LEN       24u

/* ═══════════════════════════════════════════════════════════════════════════
 *  SD CARD
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_SD_MOUNT_RETRIES        3u
#define CFG_SD_DRIVE_PATH           "0:"

/* ═══════════════════════════════════════════════════════════════════════════
 *  SYSTEM TIMER  — 1 ms tick driven by Timer1
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_SYSTICK_PERIOD_MS       1u

/* ═══════════════════════════════════════════════════════════════════════════
 *  FIRMWARE IDENTIFICATION
 * ═══════════════════════════════════════════════════════════════════════════ */
#define CFG_FW_VERSION_STRING       "PIC32MM-EDAC 2.0"
#define CFG_BOARD_NAME              "pic32mm_usb_curiosity"

#endif /* APP_CONFIG_H */
