/*******************************************************************************
  System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
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

#include <xc.h>
#include "ISR_Support.h"

    .extern  TIMER_3_Handler

    .section   .vector_14,code, keep
    .equ     __vector_dispatch_14, IntVectorTIMER_3_Handler
    .global  __vector_dispatch_14
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorTIMER_3_Handler

IntVectorTIMER_3_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_3_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorTIMER_3_Handler
    .extern  UART1_FAULT_Handler

    .section   .vector_112,code, keep
    .equ     __vector_dispatch_112, IntVectorUART1_FAULT_Handler
    .global  __vector_dispatch_112
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_FAULT_Handler

IntVectorUART1_FAULT_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_FAULT_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_FAULT_Handler
    .extern  UART1_RX_Handler

    .section   .vector_113,code, keep
    .equ     __vector_dispatch_113, IntVectorUART1_RX_Handler
    .global  __vector_dispatch_113
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_RX_Handler

IntVectorUART1_RX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_RX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_RX_Handler
    .extern  UART1_TX_Handler

    .section   .vector_114,code, keep
    .equ     __vector_dispatch_114, IntVectorUART1_TX_Handler
    .global  __vector_dispatch_114
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUART1_TX_Handler

IntVectorUART1_TX_Handler:
    portSAVE_CONTEXT
    la    s6,  UART1_TX_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUART1_TX_Handler
    .extern  I2C1_BUS_Handler

    .section   .vector_115,code, keep
    .equ     __vector_dispatch_115, IntVectorI2C1_BUS_Handler
    .global  __vector_dispatch_115
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorI2C1_BUS_Handler

IntVectorI2C1_BUS_Handler:
    portSAVE_CONTEXT
    la    s6,  I2C1_BUS_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorI2C1_BUS_Handler
    .extern  I2C1_MASTER_Handler

    .section   .vector_117,code, keep
    .equ     __vector_dispatch_117, IntVectorI2C1_MASTER_Handler
    .global  __vector_dispatch_117
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorI2C1_MASTER_Handler

IntVectorI2C1_MASTER_Handler:
    portSAVE_CONTEXT
    la    s6,  I2C1_MASTER_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorI2C1_MASTER_Handler
    .extern  USB_Handler

    .section   .vector_132,code, keep
    .equ     __vector_dispatch_132, IntVectorUSB_Handler
    .global  __vector_dispatch_132
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUSB_Handler

IntVectorUSB_Handler:
    portSAVE_CONTEXT
    la    s6,  USB_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUSB_Handler
    .extern  USB_DMA_Handler

    .section   .vector_133,code, keep
    .equ     __vector_dispatch_133, IntVectorUSB_DMA_Handler
    .global  __vector_dispatch_133
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorUSB_DMA_Handler

IntVectorUSB_DMA_Handler:
    portSAVE_CONTEXT
    la    s6,  USB_DMA_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorUSB_DMA_Handler
    .extern  SQI1_Handler

    .section   .vector_169,code, keep
    .equ     __vector_dispatch_169, IntVectorSQI1_Handler
    .global  __vector_dispatch_169
    .set     nomicromips
    .set     noreorder
    .set     nomips16
    .set     noat
    .ent  IntVectorSQI1_Handler

IntVectorSQI1_Handler:
    portSAVE_CONTEXT
    la    s6,  SQI1_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end   IntVectorSQI1_Handler
