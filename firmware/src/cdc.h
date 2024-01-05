/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    cdc.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "CDC_Initialize" and "CDC_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "CDC_STATES" definition).  Both
    are defined here for convenience.
 *******************************************************************************/

#ifndef _CDC_H
#define _CDC_H

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
    // DOM-IGNORE-END

    // *****************************************************************************
    // *****************************************************************************
    // Section: Type Definitions
    // *****************************************************************************
    // *****************************************************************************
    // *****************************************************************************

#define  LED_Off()
#define  LED_On()

#define APP_READ_BUFFER_SIZE 512

    /* Application states

      Summary:
        Application states enumeration

      Description:
        This enumeration defines the valid application states.  These states
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
        
        uint8_t frame[768];

    } APP_CDC_DATA;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Callback Routines
    // *****************************************************************************
    // *****************************************************************************

    void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * pData, uintptr_t context);
    USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler(USB_DEVICE_CDC_INDEX index, USB_DEVICE_CDC_EVENT event, void* pData, uintptr_t userData);

    // *****************************************************************************
    // *****************************************************************************
    // Section: Application Initialization and State Machine Functions
    // *****************************************************************************
    // *****************************************************************************

    /*******************************************************************************
      Function:
        void CDC_Initialize ( void )

      Summary:
         MPLAB Harmony application initialization routine.

      Description:
        This function initializes the Harmony application.  It places the
        application in its initial state and prepares it to run so that its
        CDC_Tasks function can be called.

      Precondition:
        All other system initialization routines should be called before calling
        this routine (in "SYS_Initialize").

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        CDC_Initialize();
        </code>

      Remarks:
        This routine must be called from the SYS_Initialize function.
     */

    void CDC_Initialize(void);


    /*******************************************************************************
      Function:
        void CDC_Tasks ( void )

      Summary:
        MPLAB Harmony Demo application tasks function

      Description:
        This routine is the Harmony Demo application's tasks function.  It
        defines the application's state machine and core logic.

      Precondition:
        The system and application initialization ("SYS_Initialize") should be
        called before calling this.

      Parameters:
        None.

      Returns:
        None.

      Example:
        <code>
        CDC_Tasks();
        </code>

      Remarks:
        This routine must be called from SYS_Tasks() routine.
     */

    void CDC_Tasks(void);

    //DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _CDC_H */

/*******************************************************************************
 End of File
 */

