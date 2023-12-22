/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    vl53l5cx.c

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

#include "vl53l5cx.h"

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
    This structure should be initialized by the VL53L5CX_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

VL53L5CX_DATA vl53l5cxData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

void ToF_Interrupt_Handler(EXTERNAL_INT_PIN pin, uintptr_t context) {
    VL53L5CX_DATA *p_vl53l5cxData = (VL53L5CX_DATA*) context;

    if (pin == EXTERNAL_INT_1) {

    } else if (pin == EXTERNAL_INT_2) {

    }
}


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
    void VL53L5CX_Initialize ( void )

  Remarks:
    See prototype in vl53l5cx.h.
 */

void VL53L5CX_Initialize(void) {
    /* Place the App state machine in its initial state. */
    vl53l5cxData.state = VL53L5CX_STATE_INIT;
    vl53l5cxData.drvI2CHandle = DRV_HANDLE_INVALID;
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void VL53L5CX_Tasks(void) {
    uint8_t status, loop, isAlive, isReady, i;

    /* Check the application's current state. */
    switch (vl53l5cxData.state) {
            /* Application's initial state. */
        case VL53L5CX_STATE_INIT:
            // Register interrupt handler
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_1, ToF_Interrupt_Handler, (const uintptr_t) &vl53l5cxData);
            EVIC_ExternalInterruptEnable(EXTERNAL_INT_1);

            // Register interrupt handler
            EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_2, ToF_Interrupt_Handler, (const uintptr_t) &vl53l5cxData);
            EVIC_ExternalInterruptEnable(EXTERNAL_INT_2);

            // Reset ToFs
            TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
            TOF_LPn_RIGHT_OutputEnable();
            TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
            TOF_LPn_LEFT_OutputEnable();

            TOF_RST_RIGHT_Set(); // Logic 1 keep in reset
            TOF_RST_RIGHT_OutputEnable();
            TOF_RST_LEFT_Set(); // Logic 1 keep in reset
            TOF_RST_LEFT_OutputEnable();

            vTaskDelay(100U / portTICK_PERIOD_MS);
            vl53l5cxData.state = VL53L5CX_STATE_OPEN_DRV_I2C;
            break;


        case VL53L5CX_STATE_OPEN_DRV_I2C:
            vl53l5cxData.drvI2CHandle = DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);
            if (vl53l5cxData.drvI2CHandle != DRV_HANDLE_INVALID) {
                logDebug("DRV_I2C ToF opened\r\n");
                vl53l5cxData.state = VL53L5CX_STATE_POWER_ON;
            } else {
                logFatal("Cannot open DRV_I2C\r\n");
                vl53l5cxData.state = VL53L5CX_STATE_ERROR;
            }
            break;

        case VL53L5CX_STATE_POWER_ON:
            vl53l5cxData.Dev.platform.address = VL53L5CX_DEFAULT_I2C_ADDRESS;
            vl53l5cxData.Dev.platform.device = VL53L5CX_LEFT;
            vl53l5cxData.Dev.platform.drvI2CHandle = vl53l5cxData.drvI2CHandle;

            TOF_LPn_LEFT_Clear();
            TOF_LPn_LEFT_OutputEnable();
            TOF_LPn_RIGHT_Clear();
            TOF_LPn_RIGHT_OutputEnable();

            TOF_RST_LEFT_Clear();
            TOF_RST_LEFT_OutputEnable();
            TOF_RST_RIGHT_Clear();
            TOF_RST_RIGHT_OutputEnable();

            Reset_Sensor(&vl53l5cxData.Dev.platform);

            /*********************************/
            /*   Power on sensor and init    */
            /*********************************/

            /* (Optional) Check if there is a VL53L5CX sensor connected */
            status = vl53l5cx_is_alive(&vl53l5cxData.Dev, &isAlive);
            if (!isAlive || status) {
                logError("VL53L5CX not detected at requested address\r\n");
                vl53l5cxData.state = VL53L5CX_STATE_ERROR;
            } else {
                /* (Mandatory) Init VL53L5CX sensor */
                status = vl53l5cx_init(&vl53l5cxData.Dev);
                if (status) {
                    logFatal("VL53L5CX ULD Loading failed\r\n");
                    vl53l5cxData.state = VL53L5CX_STATE_ERROR;
                } else {
                    logDebug("VL53L5CX ULD ready ! (Version : %s)\r\n", VL53L5CX_API_REVISION);
                    vl53l5cxData.state = VL53L5CX_STATE_START_RANGING;
                }
            }
            break;

        case VL53L5CX_STATE_START_RANGING:
            status = vl53l5cx_start_ranging(&vl53l5cxData.Dev);
            break;

        case VL53L5CX_STATE_SERVICE_TASKS:
        {
            /* Use polling function to know when a new measurement is ready.
             * Another way can be to wait for HW interrupt raised on PIN A3
             * (GPIO 1) when a new measurement is ready */

            status = vl53l5cx_check_data_ready(&vl53l5cxData.Dev, &isReady);

            if (isReady) {
                vl53l5cx_get_ranging_data(&vl53l5cxData.Dev, &vl53l5cxData.Results);

                /* As the sensor is set in 4x4 mode by default, we have a total 
                 * of 16 zones to print. For this example, only the data of first zone are 
                 * print */
                logDebug("Print data no : %3u\r\n", vl53l5cxData.Dev.streamcount);
                for (i = 0; i < 16; i++) {
                    logDebug("Zone : %3d, Status : %3u, Distance : %4d mm\r\n",
                            i,
                            vl53l5cxData.Results.target_status[VL53L5CX_NB_TARGET_PER_ZONE * i],
                            vl53l5cxData.Results.distance_mm[VL53L5CX_NB_TARGET_PER_ZONE * i]);
                }
                logDebug("\r\n");
                loop++;
            }

            /* Wait a few ms to avoid too high polling (function in platform
             * file, not in API) */
            WaitMs(&(vl53l5cxData.Dev.platform), 5);

            vTaskDelay(100U / portTICK_PERIOD_MS);
            break;
        }

            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
