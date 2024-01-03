/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    bgt60.c

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

#include "bgt60.h"

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
    This structure should be initialized by the BGT60_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

BGT60_DATA bgt60Data;

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
    void BGT60_Initialize ( void )

  Remarks:
    See prototype in bgt60.h.
 */

void BGT60_Initialize(void) {
    /* Place the App state machine in its initial state. */
    bgt60Data.state = BGT60_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void BGT60_Tasks ( void )

  Remarks:
    See prototype in bgt60.h.
 */

void BGT60_Tasks(void) {

    /* Check the application's current state. */
    switch (bgt60Data.state) {
            /* Application's initial state. */
        case BGT60_STATE_INIT:
        {
            bool appInitialized = true;

            // Enable TXU0304 converter
            EN_IO_OutputEnable();
            EN_IO_Set();

            // Check SPI is free
            while (SPI1_IsBusy()) {
                vTaskDelay(100U / portTICK_PERIOD_MS);
            }

            CS_LEFT_OutputEnable();
            CS_RIGHT_OutputEnable();

            RST_LEFT_OutputEnable();
            RST_RIGHT_OutputEnable();


            while (true) {
                // CS_LEFT_Toggle();
                // CS_RIGHT_Toggle();
                if (!SPI1_IsBusy()) {
                    uint8_t data = 0xAA;
                    SPI1_Write(&data, 1);
                }

                // RST_LEFT_Toggle();
                // RST_RIGHT_Toggle();
                vTaskDelay(100U / portTICK_PERIOD_MS);
            }


            if (appInitialized) {

                bgt60Data.state = BGT60_STATE_SERVICE_TASKS;
            }
            break;
        }

        case BGT60_STATE_SERVICE_TASKS:
        {

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
