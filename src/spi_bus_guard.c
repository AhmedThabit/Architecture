#include "spi_bus_guard.h"

#include "definitions.h" /* GPIO + SPI3 PLIB + Harmony symbols */

/* Your CS macros from MCC */
#ifndef GPIO_RC11_SD_SS3_Set
#warning "GPIO_RC11_SD_SS3_* macros not found. Include the MCC gpio header in definitions.h."
#endif

#ifndef GPIO_RA9_FL_SS3_Set
#warning "GPIO_RA9_FL_SS3_* macros not found. Include the MCC gpio header in definitions.h."
#endif

/* --- internal state --- */
static volatile SPI_DEV g_owner = SPI_DEV_NONE;

static void deselect_all(void)
{
    /* Make sure both devices are deselected */
    GPIO_RA9_FL_SS3_Set();   /* FLASH CS high */
    GPIO_RC11_SD_SS3_Set();  /* SD CS high */
}

void SPIBus_Init(void)
{
    g_owner = SPI_DEV_NONE;
    deselect_all();
}

/* Minimal cooperative guard */
bool SPIBus_TryAcquire(SPI_DEV dev)
{
    if (dev == SPI_DEV_NONE) return false;

    if (g_owner != SPI_DEV_NONE && g_owner != dev)
    {
        return false;
    }

    g_owner = dev;

    /* Ensure the other device is deselected before any transaction */
    if (dev == SPI_DEV_SD)
    {
        GPIO_RA9_FL_SS3_Set();
        /* SD driver controls SD CS during transfers, but keep it high at idle */
        GPIO_RC11_SD_SS3_Set();
    }
    else if (dev == SPI_DEV_FLASH)
    {
        GPIO_RC11_SD_SS3_Set();
        GPIO_RA9_FL_SS3_Set();
    }

    return true;
}

void SPIBus_Release(SPI_DEV dev)
{
    if (g_owner == dev)
    {
        deselect_all();
        g_owner = SPI_DEV_NONE;
    }
}

/* -------- Optional SPI speed switching --------
 * If your generated SPI3 PLIB provides SPI3_TransferSetup(), we can switch clock.
 * Many Harmony PLIBs also provide SPI3_ClockFrequencyGet() or a SYS_CLK helper.
 *
 * If your project doesn't have those, these functions will compile but do nothing.
 */

static bool try_set_speed(uint32_t hz)
{
#if defined(SPI3_TransferSetup)
    SPI_TRANSFER_SETUP setup;
    setup.clockFrequency = hz;
    setup.dataBits       = SPI_DATA_BITS_8;
    setup.clockPolarity  = SPI_CLOCK_POLARITY_IDLE_LOW;
    setup.clockPhase     = SPI_CLOCK_PHASE_LEADING_EDGE;

    /* Find a source clock value */
    uint32_t src = 0;
    #if defined(SPI3_ClockFrequencyGet)
        src = SPI3_ClockFrequencyGet();
    #elif defined(SPI3_CLOCK_FrequencyGet)
        src = SPI3_CLOCK_FrequencyGet();
    #elif defined(SYS_CLK_PeripheralFrequencyGet) && defined(CLK_BUS_PERIPHERAL_2)
        src = SYS_CLK_PeripheralFrequencyGet(CLK_BUS_PERIPHERAL_2);
    #else
        /* If you don't have any of the above, leave src = 0; some PLIBs ignore it. */
        src = 0;
    #endif

    return SPI3_TransferSetup(&setup, src);
#else
    (void)hz;
    return false;
#endif
}

/* SD cards must start at <= 400 kHz during init */
void SPIBus_SetSpeed_SDInit(void)
{
    (void)try_set_speed(400000u);
}

/* After init most SD cards are fine at several MHz (start conservative) */
void SPIBus_SetSpeed_SDFast(void)
{
    (void)try_set_speed(8000000u);
}

/* Flash chips often support higher (keep conservative too) */
void SPIBus_SetSpeed_FlashFast(void)
{
    (void)try_set_speed(12000000u);
}
