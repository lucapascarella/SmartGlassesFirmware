/*******************************************************************************
 System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt.c

  Summary:
    Interrupt vectors mapping

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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
#include "configuration.h"
#include "interrupts.h"
#include "definitions.h"



// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


/* All the handlers are defined here.  Each will call its PLIB-specific function. */
// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector declarations
// *****************************************************************************
// *****************************************************************************
void ToF_Right_Handler (void);
void ToF_Left_Handler (void);
void TIMER_3_Handler (void);
void TIMER_7_Handler (void);
void TIMER_9_Handler (void);
void SPI1_RX_Handler (void);
void SPI1_TX_Handler (void);
void UART1_FAULT_Handler (void);
void UART1_RX_Handler (void);
void UART1_TX_Handler (void);
void I2C1_BUS_Handler (void);
void I2C1_MASTER_Handler (void);
void USB_Handler (void);
void USB_DMA_Handler (void);
void SQI1_Handler (void);


// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector definitions
// *****************************************************************************
// *****************************************************************************


void ToF_Right_Handler (void)
{
    EXTERNAL_1_InterruptHandler();
}

void ToF_Left_Handler (void)
{
    EXTERNAL_2_InterruptHandler();
}

void TIMER_3_Handler (void)
{
    TIMER_3_InterruptHandler();
}

/*
 * 10kHz Timer based on TMR7
 */
volatile unsigned long ulHighFrequencyTimerTicks = 0;

/*
 * Implements Shadow Register Sets to speed up the interrupt handling
 */
void __ISR(_TIMER_7_VECTOR, ipl7SRS) TIMER_7_Handler(void) {
    IFS1CLR = _IFS1_T7IF_MASK;
    ulHighFrequencyTimerTicks++;
}
//void TIMER_7_Handler (void)
//{
//    TIMER_7_InterruptHandler();
//}

void TIMER_9_Handler (void)
{
    TIMER_9_InterruptHandler();
}

void SPI1_RX_Handler (void)
{
    SPI1_RX_InterruptHandler();
}

void SPI1_TX_Handler (void)
{
    SPI1_TX_InterruptHandler();
}

void UART1_FAULT_Handler (void)
{
    UART1_FAULT_InterruptHandler();
}

void UART1_RX_Handler (void)
{
    UART1_RX_InterruptHandler();
}

void UART1_TX_Handler (void)
{
    UART1_TX_InterruptHandler();
}

void I2C1_BUS_Handler (void)
{
    I2C1_BUS_InterruptHandler();
}

void I2C1_MASTER_Handler (void)
{
    I2C1_MASTER_InterruptHandler();
}

void USB_Handler (void)
{
    DRV_USBHS_InterruptHandler();
}

void USB_DMA_Handler (void)
{
    DRV_USBHS_DMAInterruptHandler();
}

void SQI1_Handler (void)
{
    SQI1_InterruptHandler();
}





/*******************************************************************************
 End of File
*/
