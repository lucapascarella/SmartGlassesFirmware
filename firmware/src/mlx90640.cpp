/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    mlx90640.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "mlx90640.h"

#undef LOG_LEVEL
#define LOG_LEVEL   LOG_DEBUG
#include "log.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the MLX90640_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

MLX90640_DATA mlx90640Data;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void MLX90640_Initialize ( void )

  Remarks:
    See prototype in mlx90640.h.
 */

void MLX90640_Initialize(void) {
    /* Place the App state machine in its initial state. */
    mlx90640Data.state = MLX90640_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void MLX90640_Tasks ( void )

  Remarks:
    See prototype in mlx90640.h.
 */

void MLX90640_Tasks(void) {

    /* Check the application's current state. */
    switch (mlx90640Data.state) {
            /* Application's initial state. */
        case MLX90640_STATE_INIT:
        {
            bool appInitialized = true;
            if (appInitialized) {

                mlx90640Data.state = MLX90640_STATE_OPEN_DRV_I2C;
            }
            break;
        }

        case MLX90640_STATE_OPEN_DRV_I2C:
            mlx90640Data.drvI2CHandle = DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);
            if (mlx90640Data.drvI2CHandle != DRV_HANDLE_INVALID) {
                logDebug("DRV_I2C MLX90640 opened\r\n");

                uint8_t read_buffer[2];

                uint8_t write_buffer[2];
                write_buffer[1] = 24;
                write_buffer[0] = 07 >> 8;

                bool rtn = DRV_I2C_WriteReadTransfer(mlx90640Data.drvI2CHandle, MLX90640_I2C_ADDRESS, write_buffer, 2, read_buffer, 1);
                if (rtn) {
                    logDebug("DRV_I2C MLX90640 opened\r\n");
                    mlx90640Data.state = MLX90640_STATE_SERVICE_TASKS;
                } else {
                    logFatal("Cannot open DRV_I2C for MLX90640\r\n");
                    mlx90640Data.state = MLX90640_STATE_ERROR;
                }
            } else {
                logFatal("Cannot open DRV_I2C for MLX90640\r\n");
                mlx90640Data.state = MLX90640_STATE_ERROR;
            }
            break;

        case MLX90640_STATE_SERVICE_TASKS:
        {
            vTaskDelay(100U / portTICK_PERIOD_MS);
            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            vTaskDelay(1000U / portTICK_PERIOD_MS);
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
