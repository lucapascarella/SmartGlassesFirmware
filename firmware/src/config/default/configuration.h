/*******************************************************************************
  System Configuration Header

  File Name:
    configuration.h

  Summary:
    Build-time configuration header for the system defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    This configuration header must not define any prototypes or data
    definitions (or include any files that do).  It only provides macro
    definitions for build-time configuration options

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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
/*  This section Includes other configuration headers necessary to completely
    define this configuration.
*/

#include "user.h"
#include "device.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: System Configuration
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Service Configuration
// *****************************************************************************
// *****************************************************************************
/* TIME System Service Configuration Options */
#define SYS_TIME_INDEX_0                            (0)
#define SYS_TIME_MAX_TIMERS                         (5)
#define SYS_TIME_HW_COUNTER_WIDTH                   (32)
#define SYS_TIME_TICK_FREQ_IN_HZ                    (1525.90218)

#define SYS_CONSOLE_INDEX_0                       0





#define SYS_CMD_ENABLE
#define SYS_CMD_DEVICE_MAX_INSTANCES       SYS_CONSOLE_DEVICE_MAX_INSTANCES
#define SYS_CMD_PRINT_BUFFER_SIZE          1024U
#define SYS_CMD_BUFFER_DMA_READY

/* Command System Service RTOS Configurations*/
#define SYS_CMD_RTOS_STACK_SIZE                256
#define SYS_CMD_RTOS_TASK_PRIORITY             1


#define SYS_DEBUG_ENABLE
#define SYS_DEBUG_GLOBAL_ERROR_LEVEL       SYS_ERROR_DEBUG
#define SYS_DEBUG_BUFFER_DMA_READY
#define SYS_DEBUG_USE_CONSOLE


#define SYS_CONSOLE_DEVICE_MAX_INSTANCES   			(1U)
#define SYS_CONSOLE_UART_MAX_INSTANCES 	   			(1U)
#define SYS_CONSOLE_USB_CDC_MAX_INSTANCES 	   		(0U)
#define SYS_CONSOLE_PRINT_BUFFER_SIZE        		(4096U)




// *****************************************************************************
// *****************************************************************************
// Section: Driver Configuration
// *****************************************************************************
// *****************************************************************************
/* I2C Driver Instance 0 Configuration Options */
#define DRV_I2C_INDEX_0                       0
#define DRV_I2C_CLIENTS_NUMBER_IDX0           2
#define DRV_I2C_CLOCK_SPEED_IDX0              400000

/* I2C Driver Common Configuration Options */
#define DRV_I2C_INSTANCES_NUMBER              (1U)


/* SST26 Driver Instance Configuration */
#define DRV_SST26_INDEX                 (0U)
#define DRV_SST26_CLIENTS_NUMBER        (1U)
#define DRV_SST26_START_ADDRESS         (0x0U)
#define DRV_SST26_PAGE_SIZE             (256U)
#define DRV_SST26_ERASE_BUFFER_SIZE     (4096U)
#define DRV_SST26_BUFF_DESC_NUMBER      (10U)

/* IS67 Driver Instance Configuration */
#define DRV_IS67_INDEX                  (0U)
#define DRV_IS67_CLIENTS_NUMBER         (1U)
#define DRV_IS67_START_ADDRESS          (0x0U)
#define DRV_IS67_BURST_SIZE             (1024U) /* Default 1024 */
//#define DRV_IS67_ERASE_BUFFER_SIZE      (4096U)
#define DRV_IS67_BUFF_DESC_NUMBER       (10U)



// *****************************************************************************
// *****************************************************************************
// Section: Middleware & Other Library Configuration
// *****************************************************************************
// *****************************************************************************
/* Number of Endpoints used */
#define DRV_USBHS_ENDPOINTS_NUMBER                        4

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER                         1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE                          64


/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER                     1


/* CDC Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED                 3

/*** USB Driver Configuration ***/

/* Maximum USB driver instances */
#define DRV_USBHS_INSTANCES_NUMBER                        1

/* Interrupt mode enabled */
#define DRV_USBHS_INTERRUPT_MODE                          true


/* Enables Device Support */
#define DRV_USBHS_DEVICE_SUPPORT                          true

/* Disable Host Support */
#define DRV_USBHS_HOST_SUPPORT                            false





/* Alignment for buffers that are submitted to USB Driver*/ 
#define USB_ALIGN  CACHE_ALIGN  __ALIGNED(16)


#define ENABLE_ADDRESS_FIRST


// *****************************************************************************
// *****************************************************************************
// Section: Application Configuration
// *****************************************************************************
// *****************************************************************************


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // CONFIGURATION_H
/*******************************************************************************
 End of File
*/
