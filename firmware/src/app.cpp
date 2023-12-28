/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

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

#include "app.h"
#include "sys_tasks.h"
#include "sensors/MCP9000.h"

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
    This structure should be initialized by the APP_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************




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
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {

    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {

            SYS_DEBUG_ErrorLevelSet(SYS_ERROR_DEBUG);
            /*
             * Clean the console
             */
            logDebug("\a\033[1;1H\033[2J");
            logDebug("Hello World!\r\n");

            // Enable 3V3
            EN_3V3_OutputEnable();
            EN_3V3_Set();

            // Enable 1V8
            //EN_1V8_OutputEnable();
            //EN_1V8_Set();


            //                appData.handle = DRV_AT24_Open(DRV_AT24_INDEX, DRV_IO_INTENT_READWRITE);
            //                if (appData.handle != DRV_HANDLE_INVALID) {
            //                    DRV_AT24_EventHandlerSet(appData.handle, transferEventHandler, (const uintptr_t) &appData.mem);
            //                    memset(&appData.geometry, 0x00, sizeof (appData.geometry));
            //                    bool rtn = DRV_AT24_GeometryGet(appData.handle, &appData.geometry);
            //                    if (rtn) {
            //                        uint32_t readBlockSize = appData.geometry.readBlockSize;
            //                        uint32_t nReadBlocks = appData.geometry.readNumBlocks;
            //                        uint32_t nReadRegions = appData.geometry.readNumRegions;
            //                        uint32_t size = readBlockSize * nReadBlocks * nReadRegions;
            //                        logDebug("Detected %d bytes EEPROM\r\n", size);
            //                    } else {
            //                        logFatal("Cannot detect size of EEPROM\r\n");
            //                    }
            //                }

            //MCP9800_Initialize();


            /* Active low output */
            OCMP3_Enable();
            OCMP4_Enable();
            OCMP5_Enable();

            // Set duty cycle at 50%
            uint16_t ref = TMR3_PeriodGet() + 1;
            OCMP3_CompareSecondaryValueSet(ref / 2);
            OCMP4_CompareSecondaryValueSet(ref / 2);
            OCMP5_CompareSecondaryValueSet(ref / 2);

            TMR3_Start();

            appData.state = APP_STATE_SERVICE_TASKS;

            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {

            //            LED_SYS_Toggle();
            vTaskDelay(100U / portTICK_PERIOD_MS);
            break;
        }

            /* TODO: implement your application state machine.*/


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
