/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _MCP9800_H
#define _MCP9800_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
    // DOM-IGNORE-END

    // *****************************************************************************
    // *****************************************************************************
    // Section: Type Definitions
    // *****************************************************************************
    // *****************************************************************************
    // *****************************************************************************

#define MCP9800_BASE_ADDR                       (0x48)
//#define MCP9800_BASE_ADDR                       (0x90)

    // register addresses

    typedef enum _MCP9800_REG_POINTER {
        MCP9800_AMBIENT = 0,
        MCP9800_CONFIG = 1,
        MCP9800_HYSTERESIS = 2,
        MCP9800_LIMITSET = 3
    } MCP9800_REG_POINTER;

    typedef union {
        uint16_t val;

        struct {// LSB
            uint8_t lsb;
            uint8_t msb;
        } bytes;

    } MCP9800_REG_AMBIENT;

    typedef union {
        uint16_t val;

        struct {// LSB
            uint8_t lsb;
            uint8_t msb;
        } bytes;

        struct {// LSB
            uint8_t shutdown : 1; // 1 = Enable, 0 = Disable (Power-up default)
            uint8_t comp_int : 1; // 1 = Interrupt mode, 0 = Comparator mode (Power-up default)
            uint8_t alert_polarity : 1; // 1 = Active-high, 0 = Active-low (Power-up default)
            uint8_t fault_queue : 2; // 00 = 1 (Power-up default), 01 = 2, 10 = 4, 11 = 6
            uint8_t resolution : 2; // 00 = 9 bit or 0.5°C (Power-up default), 01 = 10 bit or 0.25°C, 10 = 11 bit or 0.125°C, 11 = 12 bit or 0.0625°C
            uint8_t one_shot : 1; // 1 = Enabled, 0 = Disabled (Power-up default)
        } bits;

    } MCP9800_REG_CONFIG;

//#define CONFIG_REG                              (1);
//
//    // bit definitions for config register
//#define ONE_SHOT                                (0x80);
//#define ADC_RES_9BITS                           (0x00);
//#define ADC_RES_10BITS                          (0x20);
//#define ADC_RES_11BITS                          (0x40);
//#define ADC_RES_12BITS                          (0x60);
//#define FAULT_QUEUE_1                           (0x00);
//#define FAULT_QUEUE_2                           (0x08);
//#define FAULT_QUEUE_4                           (0x10);
//#define FAULT_QUEUE_6                           (0x18);
//#define ALERT_POLARITY_HIGH                     (0x04);
//#define INTERRUPT_MODE                          (0x02);
//#define SHUTDOWN                                (0x01);

    typedef struct {
    } MCP9800_DATA;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Callback Routines
    // *****************************************************************************
    // *****************************************************************************
    /* These routines are called by drivers when certain events occur.
     */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Initialization and State Machine Functions
    // *****************************************************************************
    // *****************************************************************************

    /*******************************************************************************
      Function:
        void APP_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
        APP_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        APP_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void MCP9800_Initialize(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // _MCP9800_H

/*******************************************************************************
 End of File
 */
