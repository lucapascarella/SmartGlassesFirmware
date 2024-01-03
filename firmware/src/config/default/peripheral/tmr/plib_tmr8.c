/*******************************************************************************
  TMR Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_tmr8.c

  Summary
    TMR8 peripheral library source file.

  Description
    This file implements the interface to the TMR peripheral library.  This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_tmr8.h"
#include "interrupts.h"


volatile static TMR_TIMER_OBJECT tmr8Obj;


void TMR8_Initialize(void)
{
    /* Disable Timer */
    T8CONCLR = _T8CON_ON_MASK;

    /*
    SIDL = 0
    TCKPS =7
    T32   = 1
    TCS = 0
    */
    T8CONSET = 0x78;

    /* Clear counter */
    TMR8 = 0x0;

    /*Set period */
    PR8 = 195311U;

    /* Enable TMR Interrupt of odd numbered timer in 32-bit mode */
    IEC1SET = _IEC1_T9IE_MASK;

}


void TMR8_Start(void)
{
    T8CONSET = _T8CON_ON_MASK;
}


void TMR8_Stop (void)
{
    T8CONCLR = _T8CON_ON_MASK;
}

void TMR8_PeriodSet(uint32_t period)
{
    PR8  = period;
}

uint32_t TMR8_PeriodGet(void)
{
    return PR8;
}

uint32_t TMR8_CounterGet(void)
{
    return (TMR8);
}


uint32_t TMR8_FrequencyGet(void)
{
    return (390625);
}


void __attribute__((used)) TIMER_9_InterruptHandler (void)
{
    uint32_t status  = 0U;
    status = IFS1bits.T9IF;
    IFS1CLR = _IFS1_T9IF_MASK;

    if((tmr8Obj.callback_fn != NULL))
    {
        uintptr_t context = tmr8Obj.context;
        tmr8Obj.callback_fn(status, context);
    }
}


void TMR8_InterruptEnable(void)
{
    IEC1SET = _IEC1_T9IE_MASK;
}


void TMR8_InterruptDisable(void)
{
    IEC1CLR = _IEC1_T9IE_MASK;
}


void TMR8_CallbackRegister( TMR_CALLBACK callback_fn, uintptr_t context )
{
    /* Save callback_fn and context in local memory */
    tmr8Obj.callback_fn = callback_fn;
    tmr8Obj.context = context;
}
