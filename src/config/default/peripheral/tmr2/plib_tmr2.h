/*******************************************************************************
  Interface definition of Timer2 PLIB.

  Company:
    Microchip Technology Inc.

  File Name:
    plib_tmr2.h

  Summary:
    Interface definition of the Timer2 Plib.

  Description:
    This file defines the interface for the Timer2 Plib.
    It is designed to be compatible with SYS_TIME service.
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

#ifndef PLIB_TMR2_H
#define PLIB_TMR2_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

/* Timer2 Configuration:
 * - Input Clock: PBCLK3 (48 MHz on this PIC32MM)
 * - Prescaler: 1:8
 * - Timer Frequency: 48 MHz / 8 = 6 MHz = 6,000,000 Hz
 * - Period: 0xFFFF (65535) for maximum range
 * - This provides approximately 10.9ms between overflows
 */
#define TMR2_FREQUENCY              (6000000U)  // 48MHz / 8 = 6MHz
#define TMR2_PERIOD_VALUE           (0xFFFFU)   // 16-bit timer max value

/* Callback function pointer type */
typedef void (*TMR2_CALLBACK)(uint32_t status, uintptr_t context);

/* Timer2 object structure */
typedef struct
{
    TMR2_CALLBACK   callback;
    uintptr_t       context;
} TMR2_OBJECT;

// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/******************************************************************************
  Function:
    void TMR2_Initialize(void)

  Summary:
    Initializes Timer2 peripheral.

  Description:
    This function initializes the Timer2 peripheral with the configured settings.
    It sets up the prescaler, period, and disables the timer initially.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    None.
*/
void TMR2_Initialize(void);

/******************************************************************************
  Function:
    void TMR2_Start(void)

  Summary:
    Starts Timer2.

  Description:
    Enables Timer2 and starts counting.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    None.

  Returns:
    None.
*/
void TMR2_Start(void);

/******************************************************************************
  Function:
    void TMR2_Stop(void)

  Summary:
    Stops Timer2.

  Description:
    Disables Timer2 and stops counting.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    None.

  Returns:
    None.
*/
void TMR2_Stop(void);

/******************************************************************************
  Function:
    uint32_t TMR2_FrequencyGet(void)

  Summary:
    Gets the Timer2 operating frequency.

  Description:
    Returns the frequency at which Timer2 is operating.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    Timer2 frequency in Hz (6,000,000).
*/
uint32_t TMR2_FrequencyGet(void);

/******************************************************************************
  Function:
    void TMR2_PeriodSet(uint32_t period)

  Summary:
    Sets the Timer2 period value.

  Description:
    Sets the period register of Timer2. When the timer count reaches this
    value, it will reset to 0 and generate an interrupt if enabled.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    period - Period value (0 to 0xFFFF for 16-bit timer)

  Returns:
    None.
*/
void TMR2_PeriodSet(uint32_t period);

/******************************************************************************
  Function:
    void TMR2_CompareSet(uint32_t compare)

  Summary:
    Sets the Timer2 compare value.

  Description:
    Sets the period register (PR2) which acts as the compare value.
    When TMR2 count equals PR2, the timer resets and interrupt is generated.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    compare - Compare value (0 to 0xFFFF for 16-bit timer)

  Returns:
    None.
*/
void TMR2_CompareSet(uint32_t compare);

/******************************************************************************
  Function:
    uint32_t TMR2_CounterGet(void)

  Summary:
    Gets the current Timer2 counter value.

  Description:
    Returns the current value of the Timer2 counter register.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    None.

  Returns:
    Current counter value (0 to 0xFFFF).
*/
uint32_t TMR2_CounterGet(void);

/******************************************************************************
  Function:
    void TMR2_CallbackSet(TMR2_CALLBACK callback, uintptr_t context)

  Summary:
    Registers a callback function for Timer2 interrupts.

  Description:
    This function registers a callback function that will be called when
    Timer2 interrupt occurs.

  Precondition:
    TMR2_Initialize must have been called.

  Parameters:
    callback - Pointer to callback function
    context  - User-defined context value passed to callback

  Returns:
    None.
*/
void TMR2_CallbackSet(TMR2_CALLBACK callback, uintptr_t context);

#ifdef __cplusplus
}
#endif

#endif // PLIB_TMR2_H
