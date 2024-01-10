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

#define APP_MOUNT_NAME          "/mnt/sst"
#define APP_DEVICE_NAME         "/dev/mtda1"
#define APP_FS_TYPE             LITTLEFS

uint8_t CACHE_ALIGN work[512];

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
// Event Processing Technique. Event is received when operation is done.

//void APP_SysFSEventHandler(SYS_FS_EVENT event, void* eventData, uintptr_t context) {
//    switch (event) {
//        case SYS_FS_EVENT_MOUNT:
//        {
//            if (strcmp((const char *) eventData, "/mnt/sst") == 0) {
//                logDebug("Mounted\r\n");
//            }
//            break;
//        }
//
//        case SYS_FS_EVENT_MOUNT_WITH_NO_FILESYSTEM:
//        {
//            if (strcmp((const char *) eventData, "/mnt/sst") == 0) {
//                logWarning("No file system\r\n");
//            }
//            break;
//        }
//
//        case SYS_FS_EVENT_UNMOUNT:
//        {
//            if (strcmp((const char *) eventData, "/mnt/sst") == 0) {
//                logDebug("Unmounted\r\n");
//            }
//            break;
//        }
//
//        case SYS_FS_EVENT_ERROR:
//        default:
//        {
//            if (SYS_FS_Error() == SYS_FS_ERROR_NO_FILESYSTEM) {
//                logError("No file system\r\n");
//            } else {
//                logError("Mount error\r\n");
//            }
//            break;
//        }
//    }
//}



// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/* This example demonstrates how a human readable table of run time stats
information is generated from raw data provided by uxTaskGetSystemState().
The human readable table is written to pcWriteBuffer.  (see the vTaskList()
API function which actually does just this). */
static void vTaskGetRunTimeStats2(void) {
    const char states[] = {'X', 'R', 'B', 'S', 'D', 'I'};
    TaskStatus_t *pxTaskStatusArray;
    volatile UBaseType_t uxArraySize, x;
    uint32_t ulTotalRunTime, ulStatsAsPercentage;
    uint8_t state;

    /* Take a snapshot of the number of tasks in case it changes while this function is executing. */
    uxArraySize = uxTaskGetNumberOfTasks();

    /* Allocate a TaskStatus_t structure for each task. 
     * An array could be allocated statically at compile time. */
    pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof (TaskStatus_t));

    if (pxTaskStatusArray != NULL) {
        /* Generate raw status information about each task. */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalRunTime);

        /* For percentage calculations. */
        ulTotalRunTime /= 100UL;

        /* Avoid divide by zero errors. */
        if (ulTotalRunTime > 0) {
            /* For each populated position in the pxTaskStatusArray array,
            format the raw data as human readable ASCII data. */
            for (x = 0; x < uxArraySize; x++) {
                /* What percentage of the total run time has the task used?
                This will always be rounded down to the nearest integer.
                ulTotalRunTimeDiv100 has already been divided by 100. */
                ulStatsAsPercentage = pxTaskStatusArray[ x ].ulRunTimeCounter / ulTotalRunTime;
                state = pxTaskStatusArray[ x ].eCurrentState;

                if (ulStatsAsPercentage > 0UL) {
                    logPrint("%c %2lu %2lu %-15s %9u %3u%% %6d %p\r\n",
                            states[state],
                            pxTaskStatusArray[ x ].uxBasePriority,
                            pxTaskStatusArray[ x ].uxCurrentPriority,
                            pxTaskStatusArray[ x ].pcTaskName,
                            pxTaskStatusArray[ x ].ulRunTimeCounter,
                            ulStatsAsPercentage,
                            pxTaskStatusArray[ x ].usStackHighWaterMark * 4,
                            pxTaskStatusArray[ x ].pxStackBase);
                } else {
                    /* If the percentage is zero here then the task has
                    consumed less than 1% of the total run time. */
                    logPrint("%c %2lu %2lu %-15s %9u  <1%% %6d %p\r\n",
                            states[state],
                            pxTaskStatusArray[ x ].uxBasePriority,
                            pxTaskStatusArray[ x ].uxCurrentPriority,
                            pxTaskStatusArray[ x ].pcTaskName,
                            pxTaskStatusArray[ x ].ulRunTimeCounter,
                            pxTaskStatusArray[ x ].usStackHighWaterMark * 4,
                            pxTaskStatusArray[ x ].pxStackBase);
                }

                //pcWriteBuffer += strlen((char *) pcWriteBuffer);
            }
        }

        /* The array is no longer needed, free the memory it consumes. */
        vPortFree(pxTaskStatusArray);
    }
}


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

    // Client registers an event handler with file system. This is done once.
    //SYS_FS_EventHandlerSet(APP_SysFSEventHandler, (uintptr_t) NULL);
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
            bool appInitialized = true;


            if (appInitialized) {

                SYS_DEBUG_ErrorLevelSet(SYS_ERROR_DEBUG);
                /*
                 * Clean the console
                 */
                logDebug("\a\033[1;1H\033[2J");
                logDebug("Hello World!\r\n");

                // Enable 10kHz timer for FreeRTOS run time statistics
                TMR7_Start();

                // Enable 3V3
                EN_3V3_OutputEnable();
                EN_3V3_Set();

                // Enable 1V8
                EN_1V8_OutputEnable();
                EN_1V8_Set();


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

                // Wait 2 seconds before mounting the disk
                vTaskDelay(1000U / portTICK_PERIOD_MS);

                if (SYS_FS_Mount(APP_DEVICE_NAME, APP_MOUNT_NAME, APP_FS_TYPE, 0, NULL) == SYS_FS_RES_SUCCESS) {
                    /* The disk could not be mounted. Try mounting again until
                     * mount is successful. */
                    // Mount was successful. Format now.
                    SYS_FS_ERROR fs_err = SYS_FS_Error();
                    if (fs_err == SYS_FS_ERROR_OK) {
                        logDebug("Mounted\r\n");

                        SYS_FS_HANDLE fileHandle = SYS_FS_FileOpen("/mnt/sst/file.txt", (SYS_FS_FILE_OPEN_WRITE));
                        if (fileHandle != SYS_FS_HANDLE_INVALID) {
                            // Write data to the file
                            char buf[32];
                            strcpy(buf, "Hello World");
                            size_t nbytes = strlen(buf);
                            size_t bytes_written = SYS_FS_FileWrite(fileHandle, (const void *) buf, nbytes);
                            // File open is successful
                            logDebug("Written %d\r\n", bytes_written);

                            SYS_FS_FileClose(fileHandle);
                            fileHandle = SYS_FS_HANDLE_INVALID;

                            fileHandle = SYS_FS_FileOpen("/mnt/sst/file.txt", (SYS_FS_FILE_OPEN_READ));
                            if (fileHandle != SYS_FS_HANDLE_INVALID) {
                                memset(buf, 0x00, sizeof (buf));
                                size_t bytes_read = SYS_FS_FileRead(fileHandle, buf, nbytes);
                                logInfo("Read %d bytes %*s\r\n", bytes_read, bytes_read, buf);
                            } else {
                                logWarning("Cannot reopen\r\n");
                            }

                            // Now, determine the total sectors and free sectors
                            uint32_t totalSectors, freeSectors;
                            SYS_FS_RESULT res = SYS_FS_DriveSectorGet(APP_DEVICE_NAME, &totalSectors, &freeSectors);

                            if (res == SYS_FS_RES_SUCCESS) {
                                //Sector information get operation failed.
                                logDebug("Sector %d, free %d\r\n", totalSectors, freeSectors);
                            } else {
                                logWarning("Cannot get sector\r\n");
                            }
                        } else {
                            logWarning("Cannot open file\r\n");
                        }

                    } else if (fs_err == SYS_FS_ERROR_NO_FILESYSTEM || fs_err == SYS_FS_ERROR_CORRUPT) {
                        logDebug("Formatting...\r\n");
                        SYS_FS_RESULT res;
                        SYS_FS_FORMAT_PARAM opt = {0};

                        opt.fmt = SYS_FS_FORMAT_ANY;
                        opt.au_size = 0;

                        res = SYS_FS_DriveFormat(APP_DEVICE_NAME, &opt, (void *) work, 512);
                        if (res == SYS_FS_RES_FAILURE) {
                            // Format of the drive failed.
                            logError("Format failed\r\n");
                        }
                    } else {
                        logError("FsError %d\r\n", fs_err);
                    }

                } else {
                    /* Mount was successful. Open a file. */
                    logError("Mount failed\r\n");
                }

                SYS_FS_HANDLE fileHandle;


                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {
            if (SYS_DEBUG_ErrorLevelGet() >= SYS_ERROR_INFO) {
                logPrint("S BP CP Task             Abs Tick  CPU  Bytes Stk Base\r\n");
                logPrint("------------------------------------------------------\r\n");
                vTaskGetRunTimeStats2();
                logPrint("------------------------------------------------------\r\n\r\n");
                //myAllocX_PrintFreelist();
            }
            //            LED_SYS_Toggle();
            vTaskDelay(60000U / portTICK_PERIOD_MS);
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