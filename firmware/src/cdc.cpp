/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    cdc.c

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

#include "cdc.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
uint8_t __attribute__((aligned(16))) switchPromptUSB[] = "\r\nPUSH BUTTON PRESSED";

uint8_t CACHE_ALIGN cdcReadBuffer[APP_READ_BUFFER_SIZE];
uint8_t CACHE_ALIGN cdcWriteBuffer[APP_READ_BUFFER_SIZE];

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
QueueHandle_t USBDeviceTask_EventQueue_Handle;

APP_CDC_DATA appCdcData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/*************************************************
 * Application Device Layer Event Handler
 *************************************************/

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * pData, uintptr_t context) {
    uint8_t configurationValue;
    uint32_t USB_Event = 0;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    switch (event) {
        case USB_DEVICE_EVENT_POWER_REMOVED:
            appCdcData.isConfigured = false;
            /* Attach the device */
            USB_DEVICE_Detach(appCdcData.deviceHandle);
            LED_Off();
            break;

        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
            /* USB device is reset or device is deconfigured.
             * This means that USB device layer is about to deininitialize all function drivers.
             * Update LEDs to indicate reset/deconfigured state. */
            appCdcData.isConfigured = false;
            LED_Off();
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            appCdcData.isConfigured = true;
            LED_On();
            /* pData will point to the configuration. Check the configuration */
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *) pData)->configurationValue;
            if (configurationValue == 1) {
                /* Register the CDC Device application event handler here.
                 * Note how the appCdcData object pointer is passed as the user data */
                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t) & appCdcData);

                // TODO. The following line comes from the Microchip example, but it seems for double CDC device
                //USB_DEVICE_CDC_EventHandlerSet(1, APP_USBDeviceCDCEventHandler, (uintptr_t) & appCdcData);

                /*let processing USB Task know USB if configured..*/
                USB_Event = USBDEVICETASK_USBCONFIGURED_EVENT;

                xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
                portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            }
            break;

        case USB_DEVICE_EVENT_SOF:
            /* Received SOF Event */
            USB_Event = USBDEVICETASK_SOF_EVENT;

            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            LED_Off();
            break;

        case USB_DEVICE_EVENT_RESUMED:
            if (appCdcData.isConfigured == true) {
                LED_On();
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:
            /*let processing USB Task know USB is powered..*/
            USB_Event = USBDEVICETASK_USBPOWERED_EVENT;

            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            /* Attach the device */
            USB_DEVICE_Attach(appCdcData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

/************************************************
 * CDC Function Driver Application Event Handler
 ************************************************/

USB_DEVICE_CDC_EVENT_RESPONSE APP_USBDeviceCDCEventHandler(USB_DEVICE_CDC_INDEX index, USB_DEVICE_CDC_EVENT event, void* pData, uintptr_t userData) {

    APP_CDC_DATA * appCdcDataObject;
    appCdcDataObject = (APP_CDC_DATA *) userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    uint16_t * breakData;
    uint32_t USB_Event = 0;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    switch (event) {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appCdcDataObject->deviceHandle, &appCdcDataObject->appCOMPortObjects[index].getLineCodingData, sizeof (USB_CDC_LINE_CODING));
            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appCdcDataObject->deviceHandle, &appCdcDataObject->appCOMPortObjects[index].setLineCodingData, sizeof (USB_CDC_LINE_CODING));
            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *) pData;
            appCdcDataObject->appCOMPortObjects[index].controlLineStateData.dtr = controlLineStateData->dtr;
            appCdcDataObject->appCOMPortObjects[index].controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appCdcDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            breakData = (uint16_t *) pData;
            appCdcDataObject->appCOMPortObjects[index].breakData = *breakData;
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:
            USB_Event = USBDEVICETASK_READDONECOM1_EVENT;

            appCdcDataObject->numBytesRead = ((USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE*) pData)->length;

            /* Let processing USB Task know Data Read is complete */
            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appCdcDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We don't
             * do much with this data in this demo. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:
            USB_Event = USBDEVICETASK_WRITEDONECOM1_EVENT;

            /* Let processing USB Task know USB Write is complete */
            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        default:
            break;
    }
    return USB_DEVICE_CDC_EVENT_RESPONSE_NONE;
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
    void CDC_Initialize ( void )

  Remarks:
    See prototype in cdc.h.
 */

void CDC_Initialize(void) {
    USBDeviceTask_EventQueue_Handle = xQueueCreate(15, sizeof (uint32_t));

    /*dont proceed if queue was not created...*/
    if (USBDeviceTask_EventQueue_Handle == NULL) {
        while (1);
    }
    /* Initialize the application object */
    appCdcData.deviceHandle = USB_DEVICE_HANDLE_INVALID;
    appCdcData.appCOMPortObjects[0].getLineCodingData.dwDTERate = 9600;
    appCdcData.appCOMPortObjects[0].getLineCodingData.bDataBits = 8;
    appCdcData.appCOMPortObjects[0].getLineCodingData.bParityType = 0;
    appCdcData.appCOMPortObjects[0].getLineCodingData.bCharFormat = 0;

    appCdcData.ignoreSwitchPress = false;
    appCdcData.switchDebounceTimer = 0;
    appCdcData.debounceCount = 0;
    appCdcData.isConfigured = false;
}

/******************************************************************************
  Function:
    void CDC_Tasks ( void )

  Remarks:
    See prototype in cdc.h.
 */

void CDC_Tasks(void) {
    BaseType_t errStatus;
    uint32_t USBDeviceTask_State = USBDEVICETASK_OPENUSB_STATE;
    uint32_t USBDeviceTask_Event = 0;
    USB_DEVICE_CDC_TRANSFER_HANDLE COM1Read_Handle, COM1Write_Handle;


    COM1Read_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    COM1Write_Handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    int i;

    for (;;) {
        switch (USBDeviceTask_State) {
            case USBDEVICETASK_OPENUSB_STATE:
                /* Open the device layer */
                appCdcData.deviceHandle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);
                /* Check if USB Device stack returned a valid handle */
                if (appCdcData.deviceHandle != USB_DEVICE_HANDLE_INVALID) {
                    /* Register a callback with USB Device layer to get event 
                     * notification (for Endpoint 0) */
                    USB_DEVICE_EventHandlerSet(appCdcData.deviceHandle, APP_USBDeviceEventHandler, 0);
                    USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;
                    break;
                }
                /* Try again in 10 msec */
                USBDeviceTask_State = USBDEVICETASK_OPENUSB_STATE;
                vTaskDelay(10 / portTICK_PERIOD_MS);
                break;

            case USBDEVICETASK_PROCESSUSBEVENTS_STATE:
                /* Once here, USB task becomes event driven, user input will 
                  will generate events. */
                USBDeviceTask_State = USBDEVICETASK_PROCESSUSBEVENTS_STATE;

                /* Wait for an event to occur and process, see event handler*/
                errStatus = xQueueReceive(USBDeviceTask_EventQueue_Handle, &USBDeviceTask_Event, portMAX_DELAY);

                /* Make sure event was successfully received*/
                if (errStatus == pdFALSE)
                    break;

                switch (USBDeviceTask_Event) {
                    case USBDEVICETASK_USBPOWERED_EVENT:
                        USB_DEVICE_Attach(appCdcData.deviceHandle);
                        break;

                    case USBDEVICETASK_USBCONFIGURED_EVENT:
                        /*USB ready, wait for user input on either com port*/

                        /* Schedule a CDC read on COM1 */
                        USB_DEVICE_CDC_Read(USB_DEVICE_CDC_INDEX_0, &COM1Read_Handle, cdcReadBuffer, APP_READ_BUFFER_SIZE);
                        break;

                    case USBDEVICETASK_SOF_EVENT:
                        ///*USB ready, wait for user input on either com port*/
                        //if (SWITCH_STATE_PRESSED == (SWITCH_Get())) {
                        //    if (appCdcData.ignoreSwitchPress) {
                        //        /* A timer event has occurred. Update the de-bounce timer */
                        //        appCdcData.switchDebounceTimer++;
                        //
                        //        if (USB_DEVICE_ActiveSpeedGet(appCdcData.deviceHandle) == USB_SPEED_FULL) {
                        //            appCdcData.debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_FS;
                        //        } else if (USB_DEVICE_ActiveSpeedGet(appCdcData.deviceHandle) == USB_SPEED_HIGH) {
                        //            appCdcData.debounceCount = APP_USB_SWITCH_DEBOUNCE_COUNT_HS;
                        //        }
                        //        if (appCdcData.switchDebounceTimer >= appCdcData.debounceCount) {
                        //            /* Indicate that we have valid switch press. The switch is
                        //             * pressed flag will be cleared by the application tasks
                        //             * routine. We should be ready for the next key press.*/
                        //            appCdcData.switchDebounceTimer = 0;
                        //            appCdcData.ignoreSwitchPress = false;
                        //
                        //            USB_DEVICE_CDC_Write
                        //                    (
                        //                    USB_DEVICE_CDC_INDEX_0,
                        //                    &COM1Write_Handle,
                        //                    switchPromptUSB,
                        //                    sizeof (switchPromptUSB),
                        //                    USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE
                        //                    );
                        //
                        //        }
                        //    } else {
                        //        /* We have a fresh key press */
                        //        appCdcData.ignoreSwitchPress = true;
                        //        appCdcData.switchDebounceTimer = 0;
                        //    }
                        //} else {
                        //    /* No key press. Reset all the indicators. */
                        //    appCdcData.ignoreSwitchPress = false;
                        //    appCdcData.switchDebounceTimer = 0;
                        //}

                        break;

                    case USBDEVICETASK_READDONECOM1_EVENT:
                        /* Send the received data to COM2 */
                        /* Else echo each received character by adding 1 */
                        for (i = 0; i < appCdcData.numBytesRead; i++) {
                            if ((cdcReadBuffer[i] != 0x0A) && (cdcReadBuffer[i] != 0x0D)) {
                                cdcWriteBuffer[i] = cdcReadBuffer[i] + 1;
                            }
                        }
                        USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0, &COM1Write_Handle, cdcWriteBuffer, appCdcData.numBytesRead, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                        break;

                    case USBDEVICETASK_WRITEDONECOM1_EVENT:
                        /* Schedule a CDC read on COM1 */
                        USB_DEVICE_CDC_Read(USB_DEVICE_CDC_INDEX_0, &COM1Read_Handle, cdcReadBuffer, APP_READ_BUFFER_SIZE);
                        break;

                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
}


/*******************************************************************************
 End of File
 */
