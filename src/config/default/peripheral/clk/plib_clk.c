/*******************************************************************************
  SYS CLK Static Functions for Clock System Service

  Company:
    Microchip Technology Inc.

  File Name:
    plib_clk.c

  Summary:
    SYS CLK static function implementations for the Clock System Service.

  Description:
    The Clock System Service provides a simple interface to manage the
    oscillators on Microchip microcontrollers. This file defines the static
    implementation for the Clock System Service.

  Remarks:
    Static functions incorporate all system clock configuration settings as
    determined by the user via the Microchip Harmony Configurator GUI.
    It provides static version of the routines, eliminating the need for an
    object ID or object handle.

    Static single-open interfaces also eliminate the need for the open handle.

 *******************************************************************************/

/*******************************************************************************
 * Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * Subject to your compliance with these terms, you may use Microchip software
 * and any derivatives exclusively with Microchip products. It is your
 * responsibility to comply with third party license terms applicable to your
 * use of third party software (including open source software) that may
 * accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
 * ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Include Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_clk.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Functions
// *****************************************************************************
// *****************************************************************************


#define LED_On() (LATASET = (1UL<<15))
#define LED_Off() (LATACLR = (1UL<<15))

#define LED_LED1_TRIS        TRISAbits.TRISA15
#define LED_LED1_LAT         LATAbits.LATA15


#define LED_ON  1
#define LED_OFF 0

#define INPUT  1
#define OUTPUT 0


// *****************************************************************************

/* Function:
    void CLK_Initialize( void )

  Summary:
    Initializes hardware and internal data structure of the System Clock.

  Description:
    This function initializes the hardware and internal data structure of System
    Clock Service.

  Remarks:
    This is configuration values for the static version of the Clock System
    Service module is determined by the user via the MHC GUI.

    The objective is to eliminate the user's need to be knowledgeable in the
    function of the 'configuration bits' to configure the system oscillators.
 */

void CLK_Initialize(void) {
    /* unlock system for clock configuration */
    SYSKEY = 0x00000000U;
    SYSKEY = 0xAA996655U;
    SYSKEY = 0x556699AAU;

    /* Peripheral Module Disable Configuration */
    PMD1 = 0x101001U;
    PMD2 = 0xf000007U;
    PMD3 = 0x1ff00U;
    PMD4 = 0x6U;
    PMD5 = 0x1070600U;
    PMD6 = 0x1U;
    PMD7 = 0x0U;

    /* Even though SPLL is selected in FNOSC, Harmony generates #pragma code as FRCDIV, not as SPLL, in "initilization.c".
     * Switching to SPLL is done here after appropriate setting of SPLLCON register.
     * This is done to ensure we don't end-up changing PLL setting when it is ON. */
    
//    PMD5CLR = 0x01070600; // re-enable USB, I2C1/2/3, SPI2/3
//// or per-module:
//PMD5CLR = (1u<<24) | (1u<<18) | (1u<<17) | (1u<<16) | (1u<<10) | (1u<<9);


    /* Configure SPLL */
    /* DIV_2, MUL_12, PLLSRC= FRC */
    SPLLCON =  0x10080;; //0x1050080;

    /* Now switch to the PLL source */
    OSCCON = OSCCON | 0x00000101U; //NOSC = SPLL, initiate clock switch (OSWEN = 1)

    /* Wait for PLL to be ready and clock switching operation to complete */
    uint32_t status = CLKSTATbits.SPLLRDY;
    status |= CLKSTATbits.SPDIVRDY;

    //working led
    //        LED_LED1_TRIS = OUTPUT;
    //    LED_LED1_LAT = LED_ON;

  while (OSCCONbits.OSWEN != 0U) {
        // Optionally: wait for SPLL lock if needed
        while (!(CLKSTATbits.SPLLRDY && CLKSTATbits.SPDIVRDY));
    }



    //
    //        while (OSCCONbits.OSWEN != 0U) {
    //            // Optionally: wait for SPLL lock if needed
    //            while (!(CLKSTATbits.SPLLRDY && CLKSTATbits.SPDIVRDY));
    //        }

    //working    
    //    LED_LED1_TRIS = OUTPUT;
    //    LED_LED1_LAT = LED_ON;

    /* Lock system since done with clock configuration */
    SYSKEY = 0x33333333U;


    // Make RA15 digital, set as output, drive LED ON (active-high assumed)

    // 1) Ensure digital mode
    ANSELACLR = (1u << 15); // clear ANSA15 -> digital

    // 2) Make it output
    TRISACLR = (1u << 15); // 0 = output (atomic)

    // 3) Drive the latch
//    LATASET = (1u << 15); // set = 1 (atomic)
    // If your LED is active-low, use LATACLR instead:
//     LATACLR = (1u << 15);


    // working
//    LED_LED1_TRIS = OUTPUT;
    LED_LED1_LAT = LED_OFF;


}
