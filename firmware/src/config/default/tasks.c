/*******************************************************************************
 System Tasks File

  File Name:
    tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled tasks.

  Description:
    This file contains source code necessary to maintain system's polled tasks.
    It implements the "SYS_Tasks" function that calls the individual "Tasks"
    functions for all polled MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "configuration.h"
#include "definitions.h"
#include "sys_tasks.h"


// *****************************************************************************
// *****************************************************************************
// Section: RTOS "Tasks" Routine
// *****************************************************************************
// *****************************************************************************
void _USB_DEVICE_Tasks(  void *pvParameters  )
{
    while(1)
    {
                /* USB Device layer tasks routine */
        USB_DEVICE_Tasks(sysObj.usbDevObject0);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/* Handle for the APP_Tasks. */
TaskHandle_t xAPP_Tasks;

static void lAPP_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        APP_Tasks();
    }
}
/* Handle for the CDC_Tasks. */
TaskHandle_t xCDC_Tasks;

static void lCDC_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        CDC_Tasks();
    }
}
/* Handle for the VL53L5CX_Tasks. */
TaskHandle_t xVL53L5CX_Tasks;

static void lVL53L5CX_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        VL53L5CX_Tasks();
    }
}
/* Handle for the MLX90640_Tasks. */
TaskHandle_t xMLX90640_Tasks;

static void lMLX90640_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        MLX90640_Tasks();
    }
}
/* Handle for the BGT60_Tasks. */
TaskHandle_t xBGT60_Tasks;

static void lBGT60_Tasks(  void *pvParameters  )
{   
    while(true)
    {
        BGT60_Tasks();
    }
}

TaskHandle_t xSYS_CMD_Tasks;
void lSYS_CMD_Tasks(  void *pvParameters  )
{
    while(1)
    {
        SYS_CMD_Tasks();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


void _DRV_USBHS_Tasks(  void *pvParameters  )
{
    while(1)
    {
				 /* USB FS Driver Task Routine */
        DRV_USBHS_Tasks(sysObj.drvUSBHSObject);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


static void lSYS_FS_Tasks(  void *pvParameters  )
{
    while(true)
    {
        SYS_FS_Tasks();
        vTaskDelay(10U / portTICK_PERIOD_MS);
    }
}





// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/
void SYS_Tasks ( void )
{
    /* Maintain system services */
    

    (void) xTaskCreate( lSYS_CMD_Tasks,
        "SYS_CMD_TASKS",
        SYS_CMD_RTOS_STACK_SIZE,
        (void*)NULL,
        SYS_CMD_RTOS_TASK_PRIORITY,
        &xSYS_CMD_Tasks
    );



    (void) xTaskCreate( lSYS_FS_Tasks,
        "SYS_FS_TASKS",
        SYS_FS_STACK_SIZE,
        (void*)NULL,
        SYS_FS_PRIORITY,
        (TaskHandle_t*)NULL
    );



    /* Maintain Device Drivers */
    


    /* Maintain Middleware & Other Libraries */
        /* Create OS Thread for USB_DEVICE_Tasks. */
    xTaskCreate( _USB_DEVICE_Tasks,
        "USB_DEVICE_TASKS",
        1024,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );

	/* Create OS Thread for USB Driver Tasks. */
    xTaskCreate( _DRV_USBHS_Tasks,
        "DRV_USBHS_TASKS",
        1024,
        (void*)NULL,
        1,
        (TaskHandle_t*)NULL
    );



    /* Maintain the application's state machine. */
        /* Create OS Thread for APP_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lAPP_Tasks,
                "APP_Tasks",
                1024,
                NULL,
                1,
                &xAPP_Tasks);

    /* Create OS Thread for CDC_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lCDC_Tasks,
                "CDC_Tasks",
                1024,
                NULL,
                1,
                &xCDC_Tasks);

    /* Create OS Thread for VL53L5CX_Tasks. */
//    (void) xTaskCreate((TaskFunction_t) lVL53L5CX_Tasks,
//                "VL53L5CX_Tasks",
//                1024,
//                NULL,
//                1,
//                &xVL53L5CX_Tasks);

    /* Create OS Thread for MLX90640_Tasks. */
    (void) xTaskCreate((TaskFunction_t) lMLX90640_Tasks,
                "MLX90640_Tasks",
                1024,
                NULL,
                1,
                &xMLX90640_Tasks);

    /* Create OS Thread for BGT60_Tasks. */
//    (void) xTaskCreate((TaskFunction_t) lBGT60_Tasks,
//                "BGT60_Tasks",
//                1024,
//                NULL,
//                1,
//                &xBGT60_Tasks);




    /* Start RTOS Scheduler. */
    
     /**********************************************************************
     * Create all Threads for APP Tasks before starting FreeRTOS Scheduler *
     ***********************************************************************/
    vTaskStartScheduler(); /* This function never returns. */

}

/*******************************************************************************
 End of File
 */

