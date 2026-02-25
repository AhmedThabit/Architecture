#include "definitions.h"           // For GPIO macros
#include "config/default/peripheral/spi/spi_master/plib_spi3_master.h"      // For SPI3_TransferSetup, SPI_TRANSFER_SETUP
#include "sd_spi_lowlevel.h"

// You can tune these if needed
#define SD_SPI_INIT_BAUD    400000UL     // 400 kHz for SD init
#define SD_SPI_HIGH_BAUD   8000000UL     // 8 MHz for normal use

// Helper: configure SPI3 speed / mode / bits
static void SD_SPI_SetupClock(uint32_t baud)
{
    SPI_TRANSFER_SETUP setup;

    setup.clockFrequency = baud;
    setup.clockPhase     = SPI_CLOCK_PHASE_LEADING_EDGE;   // CKE = 1 :contentReference[oaicite:1]{index=1}
    setup.clockPolarity  = SPI_CLOCK_POLARITY_IDLE_LOW;    // CKP = 0 :contentReference[oaicite:2]{index=2}
    setup.dataBits       = SPI_DATA_BITS_8;                // 8-bit transfers :contentReference[oaicite:3]{index=3}

    /* spiSourceClock = 0 ? PLIB uses default 48 MHz from GUI,
       or you can pass your actual PBCLK if you prefer. :contentReference[oaicite:4]{index=4} */
    (void)SPI3_TransferSetup(&setup, 0);
}

// Initialize SPI3 for SD at low speed (init phase)
void SD_SPI_InitLowSpeed(void)
{
    // SPI3_Initialize() should already be called from SYS_Initialize().
    // Now just force it to the slow init speed.
    SD_SPI_SetupClock(SD_SPI_INIT_BAUD);

    // Make sure CS pins are outputs and deselected
    GPIO_RC11_SD_SS3_OutputEnable();
    GPIO_RA9_FL_SS3_OutputEnable();

    GPIO_RC11_SD_SS3_Set();    // SD CS high (not selected)
    GPIO_RA9_FL_SS3_Set();     // Flash CS high (not selected)
}

// Switch to higher speed for normal SD operations (after init completes)
void SD_SPI_SetHighSpeed(void)
{
    SD_SPI_SetupClock(SD_SPI_HIGH_BAUD);
}

// Full-duplex byte transfer using SPI3_WriteRead
uint8_t SD_SPI_TxRx(uint8_t d)
{
    uint8_t rx = 0xFF;
    SPI3_WriteRead(&d, 1, &rx, 1);     // uses your PLIB implementation :contentReference[oaicite:5]{index=5}
    return rx;
}

// Select SD card and deselect Flash
void SD_Select(void)
{
    GPIO_RA9_FL_SS3_Set();      // Flash CS high
    GPIO_RC11_SD_SS3_Clear();   // SD   CS low
}

// Deselect SD card
void SD_Deselect(void)
{
    GPIO_RC11_SD_SS3_Set();     // SD CS high
    // One extra clock to let card release DO
    SD_SPI_TxRx(0xFF);
}
