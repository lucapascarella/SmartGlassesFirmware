/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    mlx90640.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "MLX90640_Initialize" and "MLX90640_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "MLX90640_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _MLX90640_H
#define _MLX90640_H

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

#include "sensors/MLX90640_API.h"

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

#define MLX90640_I2C_ADDRESS 0x33

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
        determine the behavior of the application at various times.
     */

    typedef enum {
        /* Application's state machine's initial state. */
        MLX90640_STATE_INIT = 0,
        MLX90640_STATE_OPEN_DRV_I2C,
        MLX90640_STATE_GET_CURR_RES,
        MLX90640_STATE_GET_REFRESH_RATE,
        MLX90640_STATE_GET_CURRENT_MODE,
        MLX90640_STATE_DUMP_EEPROM,
        MLX90640_STATE_EXTRACT_PARAMS,
        MLX90640_STATE_SERVICE_TASKS,

        MLX90640_STATE_ERROR,

    } MLX90640_STATES;


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
        MLX90640_STATES state;
        DRV_HANDLE drvI2CHandle;
        uint16_t mlx90640Frame[834];
        uint16_t eeMLX90640[832];
        paramsMLX90640 mlx90640;
        float mlx90640Image[768];
    } MLX90640_DATA;

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
        void MLX90640_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
        MLX90640_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        MLX90640_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void MLX90640_Initialize(void);


    /*******************************************************************************
      Function:
        void MLX90640_Tasks ( void )

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
        MLX90640_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void MLX90640_Tasks(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _MLX90640_H */

/*******************************************************************************
 End of File
 */

