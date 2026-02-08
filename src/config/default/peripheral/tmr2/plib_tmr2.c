/*******************************************************************************
  Timer2 Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_tmr2.c

  Summary:
    Timer2 Source File

  Description:
    This file implements the Timer2 PLIB for PIC32MM.
    It provides a compatible interface with SYS_TIME service.
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

#include "device.h"
#include "peripheral/tmr2/plib_tmr2.h"
#include "interrupts.h"

// *****************************************************************************
// *****************************************************************************
// Section: File Scope Data
// *****************************************************************************
// *****************************************************************************

volatile static TMR2_OBJECT tmr2Obj;

// *****************************************************************************
// *****************************************************************************
// Section: TMR2 Implementation
// *****************************************************************************
// *****************************************************************************

void TMR2_Initialize(void)
{
    /* Disable Timer */
    T2CONCLR = _T2CON_ON_MASK;
    
    /* Clear the callback */
    tmr2Obj.callback = NULL;
    
    /* 
     * TMR2 Configuration for SYS_TIME:
     * - TCKPS = 011 (1:8 prescaler) 
     * - TCS = 0 (Internal PBCLK3)
     * - T32 = 0 (16-bit mode)
     * - SIDL = 0 (Continue in idle mode)
     * 
     * PBCLK3 = 48 MHz on this PIC32MM
     * Timer frequency = 48 MHz / 8 = 6 MHz
     */
    T2CON = 0x0030;  /* Prescaler 1:8 */
    
    /* Set Period to maximum for free-running counter */
    PR2 = TMR2_PERIOD_VALUE;
    
    /* Clear counter */
    TMR2 = 0;
    
    /* Clear interrupt flag */
    IFS0CLR = _IFS0_T2IF_MASK;
    
    /* Note: Interrupt priority is configured by EVIC_Initialize() */
    /* Interrupt will be enabled when TMR2_Start() is called */
}

void TMR2_Start(void)
{
    /* Clear the timer counter */
    TMR2 = 0;
    
    /* Clear interrupt flag */
    IFS0CLR = _IFS0_T2IF_MASK;
    
    /* Enable interrupt */
    IEC0SET = _IEC0_T2IE_MASK;
    
    /* Start the timer */
    T2CONSET = _T2CON_ON_MASK;
}

void TMR2_Stop(void)
{
    /* Stop the timer */
    T2CONCLR = _T2CON_ON_MASK;
    
    /* Disable interrupt */
    IEC0CLR = _IEC0_T2IE_MASK;
}

uint32_t TMR2_FrequencyGet(void)
{
    return TMR2_FREQUENCY;
}

void TMR2_PeriodSet(uint32_t period)
{
    /* Set the period register */
    /* For 16-bit timer, mask to 16 bits */
    PR2 = (uint16_t)(period & 0xFFFFU);
}

void TMR2_CompareSet(uint32_t compare)
{
    /* For Timer2, the compare is achieved using the Period Register (PR2) */
    /* When TMR2 equals PR2, the timer resets and generates an interrupt */
    TMR2_PeriodSet(compare);
}

uint32_t TMR2_CounterGet(void)
{
    /* Return current counter value */
    return (uint32_t)TMR2;
}

void TMR2_CallbackSet(TMR2_CALLBACK callback, uintptr_t context)
{
    tmr2Obj.callback = callback;
    tmr2Obj.context = context;
}

// *****************************************************************************
// *****************************************************************************
// Section: Timer2 Interrupt Handler
// *****************************************************************************
// *****************************************************************************

void __attribute__((used)) TIMER_2_InterruptHandler(void)
{
    uint32_t status;
    
    /* Read and clear the interrupt flag */
    status = IFS0bits.T2IF;
    IFS0CLR = _IFS0_T2IF_MASK;
    
    /* Call the callback if registered */
    if (tmr2Obj.callback != NULL)
    {
        uintptr_t context = tmr2Obj.context;
        tmr2Obj.callback(status, context);
    }
}

/*******************************************************************************
 End of File
*/
