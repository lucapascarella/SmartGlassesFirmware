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
#include "sensors/MLX90640_I2C_Driver.h"

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
                MLX90640_I2CInit(mlx90640Data.drvI2CHandle);
                MLX90640_I2CGeneralReset();
                mlx90640Data.state = MLX90640_STATE_GET_CURR_RES;
            } else {
                logFatal("Cannot open DRV_I2C for MLX90640\r\n");
                mlx90640Data.state = MLX90640_STATE_ERROR;
            }
            break;

        case MLX90640_STATE_GET_CURR_RES:
        {
            //MLX90640_SetResolution(MLX90640_I2C_ADDRESS, 0x00);
            int current_resolution = MLX90640_GetCurResolution(MLX90640_I2C_ADDRESS);
            uint8_t res_map[] = {16, 17, 18, 19}; // bits
            logDebug("MLX90640 resolution %d bits\r\n", res_map[current_resolution]);
            mlx90640Data.state = MLX90640_STATE_GET_REFRESH_RATE;
            break;
        }

        case MLX90640_STATE_GET_REFRESH_RATE:
        {
            //MLX90640_SetResolution(MLX90640_I2C_ADDRESS, 0x00);
            int refresh_rate = MLX90640_GetRefreshRate(MLX90640_I2C_ADDRESS);
            uint8_t res_map[] = {0, 1, 2, 4, 8, 16, 32, 64}; // Hz
            logDebug("MLX90640 refresh rate %d Hz\r\n", res_map[refresh_rate ]);
            mlx90640Data.state = MLX90640_STATE_GET_CURRENT_MODE;
            break;
        }

        case MLX90640_STATE_GET_CURRENT_MODE:
        {
            //MLX90640_SetResolution(MLX90640_I2C_ADDRESS, 0x00);
            int mode = MLX90640_GetCurMode(MLX90640_I2C_ADDRESS);
            const char *res_map[] = {"interleaved", "chess"};
            logDebug("MLX90640 reading mode %s\r\n", res_map[mode]);
            mlx90640Data.state = MLX90640_STATE_DUMP_EEPROM;
            break;
        }

        case MLX90640_STATE_DUMP_EEPROM:
        {
            int status = MLX90640_DumpEE(0x33, mlx90640Data.eeMLX90640);
            if (!status) {
                logDebug("MLX90640 EEPROM dump success\r\n");
                mlx90640Data.state = MLX90640_STATE_EXTRACT_PARAMS;
            } else {
                logFatal("MLX90640 EEPROM error\r\n");
                mlx90640Data.state = MLX90640_STATE_ERROR;
            }
        }

        case MLX90640_STATE_EXTRACT_PARAMS:
        {
            int status = MLX90640_ExtractParameters(mlx90640Data.eeMLX90640, &mlx90640Data.mlx90640);
            if (!status) {
                logDebug("MLX90640 parameters extracted\r\n");
                mlx90640Data.state = MLX90640_STATE_SERVICE_TASKS;
            } else {
                logFatal("MLX90640 cannot extract parameters\r\n");
                mlx90640Data.state = MLX90640_STATE_ERROR;
            }
        }

        case MLX90640_STATE_SERVICE_TASKS:
        {
            int status = MLX90640_SynchFrame(MLX90640_I2C_ADDRESS);
            if (!status) {
                status = MLX90640_GetFrameData(MLX90640_I2C_ADDRESS, mlx90640Data.mlx90640Frame);
                if (status >= 0) {
                    logDebug("MLX90640 read success 1\r\n");
                    status = MLX90640_GetFrameData(MLX90640_I2C_ADDRESS, mlx90640Data.mlx90640Frame);
                    if (status >= 0) {
                        logDebug("MLX90640 read success 2\r\n");
                        MLX90640_GetImage(mlx90640Data.mlx90640Frame, &mlx90640Data.mlx90640, mlx90640Data.mlx90640Image);
                        float vdd = MLX90640_GetVdd(mlx90640Data.mlx90640Frame, &mlx90640Data.mlx90640); //vdd = 3.3
                        logDebug("MLX90640 Vdd %.3f\r\n", vdd);
                        
                        
                    } else {
                        logFatal("MLX90640 read error %d\r\n", status);
                        mlx90640Data.state = MLX90640_STATE_ERROR;
                    }
                } else {
                    logFatal("MLX90640 read error\r\n");
                    mlx90640Data.state = MLX90640_STATE_ERROR;
                }
            } else {
                logFatal("MLX90640 sync error\r\n");
                mlx90640Data.state = MLX90640_STATE_ERROR;
            }
            vTaskDelay(2000U / portTICK_PERIOD_MS);
            break;
        }


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
