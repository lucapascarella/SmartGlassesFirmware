/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ram.c

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

#include "ram.h"

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
    This structure should be initialized by the RAM_Initialize function.

    Application strings and buffers are be defined outside this structure.
 */

static uint8_t CACHE_ALIGN jedec_id[4];
static uint8_t CACHE_ALIGN data[512];
RAM_DATA ramData;

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

static void print_buffer_human(uint8_t *data, uint16_t data_len) {
    logPrint("Addr.   00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\r\n");
    for (int16_t i = 0; i < data_len; i += 16) {
        logPrint("0x%04x)", i);
        for (int16_t j = 0; j < 16; j++) {
            logPrint(" %02x", data[i + j]);
        }
        logPrint("\r\n");
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void RAM_Initialize ( void )

  Remarks:
    See prototype in ram.h.
 */

void RAM_Initialize(void) {
    /* Place the App state machine in its initial state. */
    ramData.state = RAM_STATE_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void RAM_Tasks ( void )

  Remarks:
    See prototype in ram.h.
 */

void RAM_Tasks(void) {

    /* Check the application's current state. */
    switch (ramData.state) {
            /* Application's initial state. */
        case RAM_STATE_INIT:
        {
            vTaskDelay(100U / portTICK_PERIOD_MS);
            SYS_STATUS status = DRV_IS67_Status(DRV_SST26_INDEX);
            if (status == SYS_STATUS_READY) {
                logDebug("IS67 ready\r\n");
                ramData.state = RAM_STATE_OPEN_DRIVER;
            } else if (status == SYS_STATUS_BUSY) {
                vTaskDelay(100U / portTICK_PERIOD_MS);
            } else {
                logError("IS67 not ready\r\n");
                ramData.state = RAM_STATE_ERROR;
            }
            break;
        }

        case RAM_STATE_OPEN_DRIVER:
        {
            ramData.handle = DRV_IS67_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
            if (ramData.handle != DRV_HANDLE_INVALID) {
                SYS_STATUS status = DRV_IS67_Status(DRV_SST26_INDEX);
                if (status == SYS_STATUS_READY) {
                    logDebug("IS67 opened\r\n");
                    ramData.state = RAM_STATE_GET_GEOMETRY;
                } else if (status == SYS_STATUS_BUSY) {
                    vTaskDelay(100U / portTICK_PERIOD_MS);
                } else {
                    logError("IS67 not ready\r\n");
                    ramData.state = RAM_STATE_ERROR;
                }
            } else {
                logFatal("Cannot open IS67\r\n");
                ramData.state = RAM_STATE_ERROR;
            }
            break;
        }

        case RAM_STATE_GET_GEOMETRY:
        {
            bool success = DRV_IS67_GeometryGet(ramData.handle, &ramData.geometry);

            if (success) {
                logDebug("IS67 write block %d\r\n", ramData.geometry.write_blockSize);
                ramData.state = RAM_STATE_WRITE;
            } else {
                logError("Cannot get geometry\r\n");
                ramData.state = RAM_STATE_ERROR;
            }
            break;
        }

        case RAM_STATE_WRITE:
        {
            for (int16_t i = 0; i < sizeof(data); i++)
                data[i] = i;
            print_buffer_human(data, 256);
//            vTaskDelay(100U / portTICK_PERIOD_MS);

            bool success = DRV_IS67_Write(ramData.handle, data, 256, 0x000000);

            if (success) {
                logDebug("IS67 write block allocated\r\n");
                while (DRV_IS67_TransferStatusGet(ramData.handle) == DRV_IS67_TRANSFER_BUSY)
                    vTaskDelay(10U / portTICK_PERIOD_MS);
                logDebug("IS67 write block completed\r\n");
                ramData.state = RAM_STATE_READ;
                vTaskDelay(100U / portTICK_PERIOD_MS);
            } else {
                logError("Cannot get geometry\r\n");
                ramData.state = RAM_STATE_ERROR;
            }
            break;
        }

        case RAM_STATE_READ:
        {
            for (int16_t i = 0; i < sizeof(data); i++)
                data[i] = 0;

            bool success = DRV_IS67_Read(ramData.handle, data, 512, 0x000000);

            if (success) {
                logDebug("IS67 read allocated\r\n");
                while (DRV_IS67_TransferStatusGet(ramData.handle) == DRV_IS67_TRANSFER_BUSY)
                    vTaskDelay(10U / portTICK_PERIOD_MS);
                logDebug("IS67 read completed\r\n");
                print_buffer_human(data, 512);

                ramData.state = RAM_STATE_SERVICE_TASKS;
            } else {
                logError("Cannot get geometry\r\n");
                ramData.state = RAM_STATE_ERROR;
            }
            break;
        }

            //        case RAM_STATE_INIT:
            //        {
            //            vTaskDelay(100U / portTICK_PERIOD_MS);
            //            SYS_STATUS status = DRV_SST26_Status(DRV_SST26_INDEX);
            //            if (status == SYS_STATUS_READY) {
            //                logDebug("SST26 ready\r\n");
            //                ramData.state = RAM_STATE_OPEN_DRIVER;
            //            } else if (status == SYS_STATUS_BUSY) {
            //                vTaskDelay(100U / portTICK_PERIOD_MS);
            //            } else {
            //                logError("SST26 not ready\r\n");
            //                ramData.state = RAM_STATE_ERROR;
            //            }
            //            break;
            //        }
            //
            //        case RAM_STATE_OPEN_DRIVER:
            //        {
            //            ramData.handle = DRV_SST26_Open(DRV_SST26_INDEX, DRV_IO_INTENT_READWRITE);
            //            if (ramData.handle != DRV_HANDLE_INVALID) {
            //                SYS_STATUS status = DRV_SST26_Status(DRV_SST26_INDEX);
            //                if (status == SYS_STATUS_READY) {
            //                    logDebug("SST26 opened\r\n");
            //                    ramData.state = RAM_STATE_GET_GEOMETRY;
            //                } else if (status == SYS_STATUS_BUSY) {
            //                    vTaskDelay(100U / portTICK_PERIOD_MS);
            //                } else {
            //                    logError("SST26 not ready\r\n");
            //                    ramData.state = RAM_STATE_ERROR;
            //                }
            //            } else {
            //                logFatal("Cannot open SST26\r\n");
            //                ramData.state = RAM_STATE_ERROR;
            //            }
            //            break;
            //        }
            //
            //        case RAM_STATE_GET_GEOMETRY:
            //        {
            //            bool success = DRV_SST26_GeometryGet(ramData.handle, &ramData.geometry);
            //
            //            if (success) {
            //                logDebug("SST26 geometry, erase block %d\r\n", ramData.geometry.erase_blockSize);
            //                ramData.state = RAM_STATE_READ_JEDEC_ID;
            //            } else {
            //                logError("Cannot get geometry\r\n");
            //                ramData.state = RAM_STATE_ERROR;
            //            }
            //            break;
            //        }
            //
            //        case RAM_STATE_READ_JEDEC_ID:
            //        {
            //
            //            bool success = DRV_SST26_ReadJedecId(ramData.handle, &jedec_id);
            //            if (success) {
            //                logDebug("Jedec ID %x", jedec_id);
            //                ramData.state = RAM_STATE_SERVICE_TASKS;
            //            } else {
            //                logError("Cannot read Jedec ID\r\n");
            //                ramData.state = RAM_STATE_ERROR;
            //            }
            //            break;
            //        }

        case RAM_STATE_SERVICE_TASKS:
        {
            vTaskDelay(100U / portTICK_PERIOD_MS);
            break;
        }

            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            logError("Error\r\n");
            vTaskDelay(1000U / portTICK_PERIOD_MS);
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
