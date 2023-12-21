/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h UUUUUUUUU

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for TOF_LPn_RIGHT pin ***/
#define TOF_LPn_RIGHT_Set()               (LATESET = (1U<<5))
#define TOF_LPn_RIGHT_Clear()             (LATECLR = (1U<<5))
#define TOF_LPn_RIGHT_Toggle()            (LATEINV= (1U<<5))
#define TOF_LPn_RIGHT_OutputEnable()      (TRISECLR = (1U<<5))
#define TOF_LPn_RIGHT_InputEnable()       (TRISESET = (1U<<5))
#define TOF_LPn_RIGHT_Get()               ((PORTE >> 5) & 0x1U)
#define TOF_LPn_RIGHT_PIN                  GPIO_PIN_RE5

/*** Macros for TOF_RST_LEFT pin ***/
#define TOF_RST_LEFT_Set()               (LATESET = (1U<<6))
#define TOF_RST_LEFT_Clear()             (LATECLR = (1U<<6))
#define TOF_RST_LEFT_Toggle()            (LATEINV= (1U<<6))
#define TOF_RST_LEFT_OutputEnable()      (TRISECLR = (1U<<6))
#define TOF_RST_LEFT_InputEnable()       (TRISESET = (1U<<6))
#define TOF_RST_LEFT_Get()               ((PORTE >> 6) & 0x1U)
#define TOF_RST_LEFT_PIN                  GPIO_PIN_RE6

/*** Macros for TOF_LPn_LEFT pin ***/
#define TOF_LPn_LEFT_Set()               (LATESET = (1U<<7))
#define TOF_LPn_LEFT_Clear()             (LATECLR = (1U<<7))
#define TOF_LPn_LEFT_Toggle()            (LATEINV= (1U<<7))
#define TOF_LPn_LEFT_OutputEnable()      (TRISECLR = (1U<<7))
#define TOF_LPn_LEFT_InputEnable()       (TRISESET = (1U<<7))
#define TOF_LPn_LEFT_Get()               ((PORTE >> 7) & 0x1U)
#define TOF_LPn_LEFT_PIN                  GPIO_PIN_RE7

/*** Macros for LED_1 pin ***/
#define LED_1_Get()               ((PORTG >> 6) & 0x1U)
#define LED_1_PIN                  GPIO_PIN_RG6

/*** Macros for LED_2 pin ***/
#define LED_2_Get()               ((PORTG >> 7) & 0x1U)
#define LED_2_PIN                  GPIO_PIN_RG7

/*** Macros for LED_SYS pin ***/
#define LED_SYS_Get()               ((PORTG >> 8) & 0x1U)
#define LED_SYS_PIN                  GPIO_PIN_RG8

/*** Macros for CS_RIGHT pin ***/
#define CS_RIGHT_Set()               (LATGSET = (1U<<9))
#define CS_RIGHT_Clear()             (LATGCLR = (1U<<9))
#define CS_RIGHT_Toggle()            (LATGINV= (1U<<9))
#define CS_RIGHT_OutputEnable()      (TRISGCLR = (1U<<9))
#define CS_RIGHT_InputEnable()       (TRISGSET = (1U<<9))
#define CS_RIGHT_Get()               ((PORTG >> 9) & 0x1U)
#define CS_RIGHT_PIN                  GPIO_PIN_RG9

/*** Macros for REFCLKO4 pin ***/
#define REFCLKO4_Get()               ((PORTB >> 5) & 0x1U)
#define REFCLKO4_PIN                  GPIO_PIN_RB5

/*** Macros for RST_LEFT pin ***/
#define RST_LEFT_Set()               (LATBSET = (1U<<4))
#define RST_LEFT_Clear()             (LATBCLR = (1U<<4))
#define RST_LEFT_Toggle()            (LATBINV= (1U<<4))
#define RST_LEFT_OutputEnable()      (TRISBCLR = (1U<<4))
#define RST_LEFT_InputEnable()       (TRISBSET = (1U<<4))
#define RST_LEFT_Get()               ((PORTB >> 4) & 0x1U)
#define RST_LEFT_PIN                  GPIO_PIN_RB4

/*** Macros for IRQ_RIGHT pin ***/
#define IRQ_RIGHT_Get()               ((PORTB >> 3) & 0x1U)
#define IRQ_RIGHT_PIN                  GPIO_PIN_RB3

/*** Macros for TOF_INT_RIGHT pin ***/
#define TOF_INT_RIGHT_Get()               ((PORTB >> 6) & 0x1U)
#define TOF_INT_RIGHT_PIN                  GPIO_PIN_RB6

/*** Macros for TOF_INT_LEFT pin ***/
#define TOF_INT_LEFT_Get()               ((PORTB >> 7) & 0x1U)
#define TOF_INT_LEFT_PIN                  GPIO_PIN_RB7

/*** Macros for EN_IO pin ***/
#define EN_IO_Set()               (LATBSET = (1U<<8))
#define EN_IO_Clear()             (LATBCLR = (1U<<8))
#define EN_IO_Toggle()            (LATBINV= (1U<<8))
#define EN_IO_OutputEnable()      (TRISBCLR = (1U<<8))
#define EN_IO_InputEnable()       (TRISBSET = (1U<<8))
#define EN_IO_Get()               ((PORTB >> 8) & 0x1U)
#define EN_IO_PIN                  GPIO_PIN_RB8

/*** Macros for IRQ_LEFT pin ***/
#define IRQ_LEFT_Get()               ((PORTB >> 9) & 0x1U)
#define IRQ_LEFT_PIN                  GPIO_PIN_RB9

/*** Macros for JTAG_TMS pin ***/
#define JTAG_TMS_Get()               ((PORTB >> 10) & 0x1U)
#define JTAG_TMS_PIN                  GPIO_PIN_RB10

/*** Macros for JTAG_TDO pin ***/
#define JTAG_TDO_Get()               ((PORTB >> 11) & 0x1U)
#define JTAG_TDO_PIN                  GPIO_PIN_RB11

/*** Macros for JTAG_TCK pin ***/
#define JTAG_TCK_Get()               ((PORTB >> 12) & 0x1U)
#define JTAG_TCK_PIN                  GPIO_PIN_RB12

/*** Macros for JTAG_TDI pin ***/
#define JTAG_TDI_Get()               ((PORTB >> 13) & 0x1U)
#define JTAG_TDI_PIN                  GPIO_PIN_RB13

/*** Macros for EN_3V3 pin ***/
#define EN_3V3_Set()               (LATBSET = (1U<<14))
#define EN_3V3_Clear()             (LATBCLR = (1U<<14))
#define EN_3V3_Toggle()            (LATBINV= (1U<<14))
#define EN_3V3_OutputEnable()      (TRISBCLR = (1U<<14))
#define EN_3V3_InputEnable()       (TRISBSET = (1U<<14))
#define EN_3V3_Get()               ((PORTB >> 14) & 0x1U)
#define EN_3V3_PIN                  GPIO_PIN_RB14

/*** Macros for EN_1V8 pin ***/
#define EN_1V8_Set()               (LATBSET = (1U<<15))
#define EN_1V8_Clear()             (LATBCLR = (1U<<15))
#define EN_1V8_Toggle()            (LATBINV= (1U<<15))
#define EN_1V8_OutputEnable()      (TRISBCLR = (1U<<15))
#define EN_1V8_InputEnable()       (TRISBSET = (1U<<15))
#define EN_1V8_Get()               ((PORTB >> 15) & 0x1U)
#define EN_1V8_PIN                  GPIO_PIN_RB15

/*** Macros for OSC1 pin ***/
#define OSC1_Get()               ((PORTC >> 12) & 0x1U)
#define OSC1_PIN                  GPIO_PIN_RC12

/*** Macros for OSC2 pin ***/
#define OSC2_Get()               ((PORTC >> 15) & 0x1U)
#define OSC2_PIN                  GPIO_PIN_RC15

/*** Macros for USBID pin ***/
#define USBID_Get()               ((PORTF >> 3) & 0x1U)
#define USBID_PIN                  GPIO_PIN_RF3

/*** Macros for DEBUG_RX pin ***/
#define DEBUG_RX_Get()               ((PORTF >> 4) & 0x1U)
#define DEBUG_RX_PIN                  GPIO_PIN_RF4

/*** Macros for DEBUG_TX pin ***/
#define DEBUG_TX_Get()               ((PORTF >> 5) & 0x1U)
#define DEBUG_TX_PIN                  GPIO_PIN_RF5

/*** Macros for SDA pin ***/
#define SDA_Get()               ((PORTD >> 9) & 0x1U)
#define SDA_PIN                  GPIO_PIN_RD9

/*** Macros for SCL pin ***/
#define SCL_Get()               ((PORTD >> 10) & 0x1U)
#define SCL_PIN                  GPIO_PIN_RD10

/*** Macros for CS_LEFT pin ***/
#define CS_LEFT_Set()               (LATDSET = (1U<<11))
#define CS_LEFT_Clear()             (LATDCLR = (1U<<11))
#define CS_LEFT_Toggle()            (LATDINV= (1U<<11))
#define CS_LEFT_OutputEnable()      (TRISDCLR = (1U<<11))
#define CS_LEFT_InputEnable()       (TRISDSET = (1U<<11))
#define CS_LEFT_Get()               ((PORTD >> 11) & 0x1U)
#define CS_LEFT_PIN                  GPIO_PIN_RD11

/*** Macros for RTCC pin ***/
#define RTCC_Get()               ((PORTD >> 0) & 0x1U)
#define RTCC_PIN                  GPIO_PIN_RD0

/*** Macros for SOSCI pin ***/
#define SOSCI_Get()               ((PORTC >> 13) & 0x1U)
#define SOSCI_PIN                  GPIO_PIN_RC13

/*** Macros for SOSCO pin ***/
#define SOSCO_Get()               ((PORTC >> 14) & 0x1U)
#define SOSCO_PIN                  GPIO_PIN_RC14

/*** Macros for SCK pin ***/
#define SCK_Get()               ((PORTD >> 1) & 0x1U)
#define SCK_PIN                  GPIO_PIN_RD1

/*** Macros for MISO pin ***/
#define MISO_Get()               ((PORTD >> 2) & 0x1U)
#define MISO_PIN                  GPIO_PIN_RD2

/*** Macros for MOSI pin ***/
#define MOSI_Get()               ((PORTD >> 3) & 0x1U)
#define MOSI_PIN                  GPIO_PIN_RD3

/*** Macros for SQICS0 pin ***/
#define SQICS0_Get()               ((PORTD >> 4) & 0x1U)
#define SQICS0_PIN                  GPIO_PIN_RD4

/*** Macros for SQICS1 pin ***/
#define SQICS1_Get()               ((PORTD >> 5) & 0x1U)
#define SQICS1_PIN                  GPIO_PIN_RD5

/*** Macros for RST_RIGHT pin ***/
#define RST_RIGHT_Set()               (LATFSET = (1U<<0))
#define RST_RIGHT_Clear()             (LATFCLR = (1U<<0))
#define RST_RIGHT_Toggle()            (LATFINV= (1U<<0))
#define RST_RIGHT_OutputEnable()      (TRISFCLR = (1U<<0))
#define RST_RIGHT_InputEnable()       (TRISFSET = (1U<<0))
#define RST_RIGHT_Get()               ((PORTF >> 0) & 0x1U)
#define RST_RIGHT_PIN                  GPIO_PIN_RF0

/*** Macros for SQICLK pin ***/
#define SQICLK_Get()               ((PORTF >> 1) & 0x1U)
#define SQICLK_PIN                  GPIO_PIN_RF1

/*** Macros for SQID0 pin ***/
#define SQID0_Get()               ((PORTE >> 0) & 0x1U)
#define SQID0_PIN                  GPIO_PIN_RE0

/*** Macros for SQID1 pin ***/
#define SQID1_Get()               ((PORTE >> 1) & 0x1U)
#define SQID1_PIN                  GPIO_PIN_RE1

/*** Macros for SQID2 pin ***/
#define SQID2_Get()               ((PORTE >> 2) & 0x1U)
#define SQID2_PIN                  GPIO_PIN_RE2

/*** Macros for SQID3 pin ***/
#define SQID3_Get()               ((PORTE >> 3) & 0x1U)
#define SQID3_PIN                  GPIO_PIN_RE3

/*** Macros for TOF_RST_RIGHT pin ***/
#define TOF_RST_RIGHT_Set()               (LATESET = (1U<<4))
#define TOF_RST_RIGHT_Clear()             (LATECLR = (1U<<4))
#define TOF_RST_RIGHT_Toggle()            (LATEINV= (1U<<4))
#define TOF_RST_RIGHT_OutputEnable()      (TRISECLR = (1U<<4))
#define TOF_RST_RIGHT_InputEnable()       (TRISESET = (1U<<4))
#define TOF_RST_RIGHT_Get()               ((PORTE >> 4) & 0x1U)
#define TOF_RST_RIGHT_PIN                  GPIO_PIN_RE4


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/


#define    GPIO_PORT_B  (0)
#define    GPIO_PORT_C  (1)
#define    GPIO_PORT_D  (2)
#define    GPIO_PORT_E  (3)
#define    GPIO_PORT_F  (4)
#define    GPIO_PORT_G  (5)
typedef uint32_t GPIO_PORT;

typedef enum
{
    GPIO_INTERRUPT_ON_MISMATCH,
    GPIO_INTERRUPT_ON_RISING_EDGE,
    GPIO_INTERRUPT_ON_FALLING_EDGE,
    GPIO_INTERRUPT_ON_BOTH_EDGES,
}GPIO_INTERRUPT_STYLE;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/


#define     GPIO_PIN_RB0  (0U)
#define     GPIO_PIN_RB1  (1U)
#define     GPIO_PIN_RB2  (2U)
#define     GPIO_PIN_RB3  (3U)
#define     GPIO_PIN_RB4  (4U)
#define     GPIO_PIN_RB5  (5U)
#define     GPIO_PIN_RB6  (6U)
#define     GPIO_PIN_RB7  (7U)
#define     GPIO_PIN_RB8  (8U)
#define     GPIO_PIN_RB9  (9U)
#define     GPIO_PIN_RB10  (10U)
#define     GPIO_PIN_RB11  (11U)
#define     GPIO_PIN_RB12  (12U)
#define     GPIO_PIN_RB13  (13U)
#define     GPIO_PIN_RB14  (14U)
#define     GPIO_PIN_RB15  (15U)
#define     GPIO_PIN_RC12  (28U)
#define     GPIO_PIN_RC13  (29U)
#define     GPIO_PIN_RC14  (30U)
#define     GPIO_PIN_RC15  (31U)
#define     GPIO_PIN_RD0  (32U)
#define     GPIO_PIN_RD1  (33U)
#define     GPIO_PIN_RD2  (34U)
#define     GPIO_PIN_RD3  (35U)
#define     GPIO_PIN_RD4  (36U)
#define     GPIO_PIN_RD5  (37U)
#define     GPIO_PIN_RD9  (41U)
#define     GPIO_PIN_RD10  (42U)
#define     GPIO_PIN_RD11  (43U)
#define     GPIO_PIN_RE0  (48U)
#define     GPIO_PIN_RE1  (49U)
#define     GPIO_PIN_RE2  (50U)
#define     GPIO_PIN_RE3  (51U)
#define     GPIO_PIN_RE4  (52U)
#define     GPIO_PIN_RE5  (53U)
#define     GPIO_PIN_RE6  (54U)
#define     GPIO_PIN_RE7  (55U)
#define     GPIO_PIN_RF0  (64U)
#define     GPIO_PIN_RF1  (65U)
#define     GPIO_PIN_RF3  (67U)
#define     GPIO_PIN_RF4  (68U)
#define     GPIO_PIN_RF5  (69U)
#define     GPIO_PIN_RG6  (86U)
#define     GPIO_PIN_RG7  (87U)
#define     GPIO_PIN_RG8  (88U)
#define     GPIO_PIN_RG9  (89U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE   (-1)

typedef uint32_t GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
	 uint32_t xvalue = (uint32_t)value;
    GPIO_PortWrite((pin>>4U), (uint32_t)(0x1U) << (pin & 0xFU), (xvalue) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return ((((GPIO_PortRead((GPIO_PORT)(pin>>4U))) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (((GPIO_PortLatchRead((GPIO_PORT)(pin>>4U)) >> (pin & 0xFU)) & 0x1U) != 0U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((pin>>4U), (uint32_t)0x1U << (pin & 0xFU));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
