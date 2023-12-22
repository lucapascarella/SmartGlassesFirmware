/* 
 * File:   USB_CDC.h
 * Author: lucap
 *
 * Created on December 22, 2023, 7:23 AM
 */

#ifndef USB_CDC_H
#define	USB_CDC_H

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

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility
extern "C" {
#endif

#define  LED_Off()
#define  LED_On()

#define APP_READ_BUFFER_SIZE 512


    // *****************************************************************************
    /* Application States

      Summary:
        Application states 

      Description:
        This defines the valid application states.  These states
        determine the behavior of the application at various times.
     */
#define USBDEVICETASK_OPENUSB_STATE                1
#define USBDEVICETASK_ATTACHUSB_STATE              2
#define USBDEVICETASK_PROCESSUSBEVENTS_STATE       3

#define USBDEVICETASK_USBPOWERED_EVENT             1
#define USBDEVICETASK_USBCONFIGURED_EVENT          2
#define USBDEVICETASK_READDONECOM1_EVENT           3
#define USBDEVICETASK_WRITEDONECOM1_EVENT          4
#define USBDEVICETASK_SOF_EVENT                    5

    /******************************************************
     * Application COM Port Object
     ******************************************************/

    typedef struct {
        /* CDC instance number */
        USB_DEVICE_CDC_INDEX cdcInstance;

        /* Set Line Coding Data */
        USB_CDC_LINE_CODING setLineCodingData;

        /* Get Line Coding Data */
        USB_CDC_LINE_CODING getLineCodingData;

        /* Control Line State */
        USB_CDC_CONTROL_LINE_STATE controlLineStateData;

        /* Break data */
        uint16_t breakData;

    } APP_COM_PORT_OBJECT;

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
        /* Device layer handle returned by device layer open function */
        USB_DEVICE_HANDLE deviceHandle;

        /* This demo supports 1 COM port */
        APP_COM_PORT_OBJECT appCOMPortObjects[1];

        int numBytesRead;

        /* True if the switch press needs to be ignored*/
        bool ignoreSwitchPress;

        /* Switch debounce timer */
        unsigned int switchDebounceTimer;

        /* Switch debounce timer count */
        unsigned int debounceCount;

        /* Device configured state */
        bool isConfigured;

    } APP_CDC_DATA;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Callback Routines
    // *****************************************************************************
    // *****************************************************************************
    /* These routines are called by drivers when certain events occur.
     */
    void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * pData, uintptr_t context);

    USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler(USB_DEVICE_CDC_INDEX index, USB_DEVICE_CDC_EVENT event, void* pData, uintptr_t userData);

    void APP_FREERTOS_Tasks(void* p_arg);
    void APP_FREERTOS_Initialize(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif	/* USB_CDC_H */

