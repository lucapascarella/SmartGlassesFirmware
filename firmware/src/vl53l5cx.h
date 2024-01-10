/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    vl53l5cx.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "VL53L5CX_Initialize" and "VL53L5CX_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "VL53L5CX_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _VL53L5CX_H
#define _VL53L5CX_H

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
#include "sensors/vl53l5cx_api.h"

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

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
        determine the behavior of the application at various times.
     */

    typedef enum {
        /* Application's state machine's initial state. */
        VL53L5CX_STATE_INIT = 0,
        VL53L5CX_STATE_OPEN_DRV_I2C,
        VL53L5CX_STATE_POWER_ON,
        VL53L5CX_STATE_GET_RESOLUTION,
        VL53L5CX_STATE_START_RANGING,
        VL53L5CX_STATE_SERVICE_TASKS,

        VL53L5CX_STATE_ERROR,

    } VL53L5CX_STATES;


    // *****************************************************************************

    /* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
     */



    typedef struct {
        /* The application's current state */
        VL53L5CX_STATES state;
        DRV_HANDLE drvI2CHandle;
        VL53L5CX_Configuration Dev; /* Sensor configuration */
        VL53L5CX_ResultsData Results; /* Results data from VL53L5CX */

        uint8_t frame [(VL53L5CX_RESOLUTION_8X8
                *VL53L5CX_NB_TARGET_PER_ZONE)];

    } VL53L5CX_DATA;

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
    void VL53L5CX_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
    VL53L5CX_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
    VL53L5CX_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void VL53L5CX_Initialize(void);


    /*******************************************************************************
      Function:
    void VL53L5CX_Tasks ( void )

      Summary:
        MPLAB Harmony Demo application tasks function

      Description:
        This routine is the Harmony Demo application's tasks function.  It
        defines the application's state machine and core logic.

      Precondition:
        The system and application initialization ("SYS_Initialize") should be
        called before calling this.

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
    VL53L5CX_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void VL53L5CX_Tasks(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _VL53L5CX_H */

/*******************************************************************************
 End of File
 */

