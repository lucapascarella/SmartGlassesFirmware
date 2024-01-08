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

#undef LOG_LEVEL
#define LOG_LEVEL   LOG_DEBUG
#include "log.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
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

APP_CDC_DATA app_cdc_data;

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
    //    uint32_t USB_Event = 0;
    APP_CDC_EVENTS app_cdc_event;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    APP_CDC_DATA * appCdcDataObject = (APP_CDC_DATA *) context;

    switch (event) {
        case USB_DEVICE_EVENT_POWER_REMOVED:
            appCdcDataObject->isConfigured = false;
            //            /* Attach the device */
            //            USB_DEVICE_Detach(appCdcData.deviceHandle);
            //            LED_Off();

            app_cdc_event = APP_CDC_EVENT_DETACH;
            xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:
            /* USB device is reset or device is deconfigured.
             * This means that USB device layer is about to deininitialize all function drivers.
             * Update LEDs to indicate reset/deconfigured state. */
            appCdcDataObject->isConfigured = false;
            LED_Off();
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            appCdcDataObject->isConfigured = true;
            LED_On();
            /* pData will point to the configuration. Check the configuration */
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *) pData)->configurationValue;
            if (configurationValue == 1) {
                //                /* Register the CDC Device application event handler here.
                //                 * Note how the appCdcData object pointer is passed as the user data */
                //                USB_DEVICE_CDC_EventHandlerSet(APP_CDC_DEV_IDX, APP_USBDeviceCDCEventHandler, context);

                // TODO. The following line comes from the Microchip example, but it seems for double CDC device
                //USB_DEVICE_CDC_EventHandlerSet(1, APP_USBDeviceCDCEventHandler, (uintptr_t) & appCdcData);

                /*let processing USB Task know USB if configured..*/
                //                USB_Event = USBDEVICETASK_USBCONFIGURED_EVENT;
                //                xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
                //                portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);

                app_cdc_event = APP_CDC_EVENT_CONFIGURE;
                xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
                portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            }
            break;

        case USB_DEVICE_EVENT_SOF:
            /* Received SOF Event */
            //            USB_Event = USBDEVICETASK_SOF_EVENT;
            //
            //            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            //            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);

            app_cdc_event = APP_CDC_EVENT_SOF;
            xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        case USB_DEVICE_EVENT_SUSPENDED:
            LED_Off();
            break;

        case USB_DEVICE_EVENT_RESUMED:
            if (appCdcDataObject->isConfigured == true) {
                LED_On();
            }
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:
            /*let processing USB Task know USB is powered..*/
            //USB_Event = USBDEVICETASK_USBPOWERED_EVENT;

            app_cdc_event = APP_CDC_EVENT_ATTACH;
            xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            /* Attach the device */
            //USB_DEVICE_Attach(appCdcData.deviceHandle);
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

    APP_CDC_DATA * appCdcDataObject = (APP_CDC_DATA *) userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    uint16_t * breakData;
    APP_CDC_EVENTS app_cdc_event;
    //    uint32_t USB_Event = 0;
    portBASE_TYPE xHigherPriorityTaskWoken1 = pdFALSE;

    switch (event) {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appCdcDataObject->device_handle, &appCdcDataObject->appCOMPortObjects.getLineCodingData, sizeof (USB_CDC_LINE_CODING));
            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appCdcDataObject->device_handle, &appCdcDataObject->appCOMPortObjects.setLineCodingData, sizeof (USB_CDC_LINE_CODING));
            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *) pData;
            appCdcDataObject->appCOMPortObjects.controlLineStateData.dtr = controlLineStateData->dtr;
            appCdcDataObject->appCOMPortObjects.controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appCdcDataObject->device_handle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            breakData = (uint16_t *) pData;
            appCdcDataObject->appCOMPortObjects.breakData = *breakData;
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:
            //            USB_Event = USBDEVICETASK_READDONECOM1_EVENT;
            //
            //            appCdcDataObject->numBytesRead = ((USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE*) pData)->length;
            //
            //            /* Let processing USB Task know Data Read is complete */
            //            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            //            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);

            appCdcDataObject->numBytesRead = ((USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE*) pData)->length;

            app_cdc_event = APP_CDC_EVENT_READ_COMPLETE;
            xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appCdcDataObject->device_handle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We don't
             * do much with this data in this demo. */
            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:
            //            USB_Event = USBDEVICETASK_WRITEDONECOM1_EVENT;
            //
            //            /* Let processing USB Task know USB Write is complete */
            //            xQueueSendToBackFromISR(USBDeviceTask_EventQueue_Handle, &USB_Event, &xHigherPriorityTaskWoken1);
            //            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken1);


            appCdcDataObject->numBytesWritten = ((USB_DEVICE_CDC_EVENT_DATA_WRITE_COMPLETE*) pData)->length;

            app_cdc_event = APP_USB_EVENT_WRITE_COMPLETE;
            xQueueSendToBackFromISR(appCdcDataObject->event_queue, &app_cdc_event, &xHigherPriorityTaskWoken1);
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
    app_cdc_data.event_queue = xQueueCreate(15, sizeof (APP_CDC_EVENTS));

    /* Don't proceed if queue was not created...*/
    if (app_cdc_data.event_queue == NULL) {
        logFatal("Cannot allocate EventQueue FreeRTOS\r\n");
        __builtin_software_breakpoint();
        vTaskDelay(portMAX_DELAY);
    }

    /* Initialize the application object */
    app_cdc_data.device_handle = USB_DEVICE_HANDLE_INVALID;
    app_cdc_data.appCOMPortObjects.getLineCodingData.dwDTERate = 9600;
    app_cdc_data.appCOMPortObjects.getLineCodingData.bDataBits = 8;
    app_cdc_data.appCOMPortObjects.getLineCodingData.bParityType = 0;
    app_cdc_data.appCOMPortObjects.getLineCodingData.bCharFormat = 0;

    app_cdc_data.handshake_states = APP_CDC_HANDSHAKE_NOT_STARTED;

    //    appCdcData.ignoreSwitchPress = false;
    //    appCdcData.switchDebounceTimer = 0;
    //    appCdcData.debounceCount = 0;
    app_cdc_data.isConfigured = false;

    app_cdc_data.read_handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
    app_cdc_data.write_handle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Move in waiting state */
    APP_CDC_EVENTS event = APP_CDC_EVENT_OPEN;
    xQueueSendToBack(app_cdc_data.event_queue, &event, portMAX_DELAY);
    logDebug("Send Open event\r\n");
}

/******************************************************************************
  Function:
    void CDC_Tasks ( void )

  Remarks:
    See prototype in cdc.h.
 */

void CDC_Tasks(void) {
    BaseType_t errStatus;
    APP_CDC_EVENTS app_cdc_event;
    const char *cdc_welcome_msg = "Hello!\r\n";
    //    USB_DEVICE_CDC_TRANSFER_HANDLE COM1Read_Handle, cdc_write_handle;

    while (true) {
        errStatus = xQueueReceive(app_cdc_data.event_queue, &app_cdc_event, portMAX_DELAY);

        /* Make sure the event was successfully received */
        if (errStatus == pdTRUE) {
            switch (app_cdc_event) {
                case APP_CDC_EVENT_OPEN:
                {
                    bool exit = false;
                    while (!exit) {
                        /* Open the device layer or wait for it is ready */
                        app_cdc_data.device_handle = USB_DEVICE_Open(USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE);

                        /* Check if USB Device stack returned a valid handle */
                        if (app_cdc_data.device_handle != USB_DEVICE_HANDLE_INVALID) {
                            /* Register a callback with USB Device layer to get event notification (for Endpoint 0) */
                            USB_DEVICE_EventHandlerSet(app_cdc_data.device_handle, APP_USBDeviceEventHandler, (uintptr_t) & app_cdc_data);
                            exit = true;
                            logInfo("USB device opened\r\n");
                        } else {
                            /* The USB device layer is not ready yet, try again in 10 msec */
                            logDebug("USB device open not ready\r\n");
                            vTaskDelay(10 / portTICK_PERIOD_MS);
                        }
                    }
                    break;
                }

                case APP_CDC_EVENT_ATTACH:
                {
                    USB_DEVICE_Attach(app_cdc_data.device_handle);
                    logInfo("USB device attached\r\n");
                    break;
                }

                case APP_CDC_EVENT_DETACH:
                {
                    USB_DEVICE_Detach(app_cdc_data.device_handle);
                    logInfo("USB device detached\r\n");
                    break;
                }

                    /*
                     * CDC configuration events
                     */
                case APP_CDC_EVENT_CONFIGURE:
                {
                    /* Register the CDC Device application event handler here */
                    USB_DEVICE_CDC_EventHandlerSet(APP_CDC_DEV_IDX, APP_USBDeviceCDCEventHandler, (uintptr_t) & app_cdc_data);
                    logDebug("USB CDC configured\r\n");

                    APP_CDC_EVENTS event = APP_CDC_EVENT_HANDSHAKE_WAIT_WELCOME;
                    xQueueSendToBack(app_cdc_data.event_queue, &event, portMAX_DELAY);
                    logDebug("USB CDC queue handshake event\r\n");
                    break;
                }

                case APP_CDC_EVENT_SOF:
                {
                    logTrace("USB SOF event\r\n");
                    break;
                }

                    /*
                     * Data transfer handlers
                     */
                case APP_CDC_EVENT_READ_COMPLETE:
                {
                    if (app_cdc_data.handshake_states == APP_CDC_HANDSHAKE_NOT_STARTED) {
                        if (app_cdc_data.numBytesRead == 8 && !memcmp(app_cdc_data.read_buffer, cdc_welcome_msg, 8)) {
                            app_cdc_data.handshake_states == APP_CDC_HANDSHAKE_COMPLETED;
                            logTrace("CDC Handshake, welcome received\r\n");

                            // Send a welcome message
                            USB_DEVICE_CDC_Write(APP_CDC_DEV_IDX, &app_cdc_data.write_handle, cdc_welcome_msg, strlen(cdc_welcome_msg), USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                            app_cdc_data.handshake_states = APP_CDC_HANDSHAKE_CLIENT_HELLO;
                            logDebug("CDC handshake, reply to welcome\r\n");
                            
//                            // TDOO. The following is only for testing
//                            uint16_t frame_size = sizeof (app_cdc_data.frame) * sizeof (uint8_t);
//                            memset(app_cdc_data.frame, 0x00, frame_size);
//                            for (uint16_t j = 0; j < frame_size; j++) {
//                                app_cdc_data.frame[j] = rand() % 255;
//                            }
//                            CDC_send_data(app_cdc_data.frame, frame_size);
                        }
                    } else {
                        logDebug("CDC device read %d byte\r\n", app_cdc_data.numBytesRead);
                    }
                    break;


                    //                    if (app_cdc_data.numBytesRead > 0) {
                    //                        switch (cdcReadBuffer[0]) {
                    //                            case '1': // Thermal sensor
                    //                            {
                    //                                uint16_t frame_size = sizeof (app_cdc_data.frame) * sizeof (uint8_t);
                    //                                memset(app_cdc_data.frame, 0x00, frame_size);
                    //                                for (uint16_t j = 0; j < frame_size; j++) {
                    //                                    app_cdc_data.frame[j] = rand() % 255;
                    //                                }
                    //                                USB_DEVICE_CDC_Write(APP_CDC_DEV_IDX, &cdc_write_handle, app_cdc_data.frame, frame_size, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                    //                                break;
                    //                            }
                    //                            case 'T': // ToF
                    //                            {
                    //                                //USB_DEVICE_CDC_Write(APP_CDC_DEV_IDX, &COM1Write_Handle, appCdcData.frame, frame_size, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                    //                                break;
                    //                            }
                    //                        }
                    //                    }
                }

                case APP_USB_EVENT_WRITE_COMPLETE:
                {
                    if (app_cdc_data.handshake_states == APP_CDC_HANDSHAKE_COMPLETED) {
                        logDebug("CDC device wrote %d bytes\r\n", app_cdc_data.numBytesWritten);
                    }
                    break;
                }

                    /*
                     * CDC Custom Handshake
                     */
                case APP_CDC_EVENT_HANDSHAKE_WAIT_WELCOME:
                {

                    // Schedule a CDC read for welcome response message
                    app_cdc_data.handshake_states = APP_CDC_HANDSHAKE_NOT_STARTED;
                    USB_DEVICE_CDC_Read(APP_CDC_DEV_IDX, &app_cdc_data.read_handle, app_cdc_data.read_buffer, APP_READ_BUFFER_SIZE);
                    break;
                }


                default:
                {
                    // Unexpected event type
                    logError("An error occurred during the handling of the USB event %d\r\n", app_cdc_event);
                    __builtin_software_breakpoint();
                    vTaskDelay(portMAX_DELAY);
                    break;
                }

            }
        } else {
            // An error happened in the waiting queue
            logError("An error occurred during the USB event dequeue\r\n");
            __builtin_software_breakpoint();
            vTaskDelay(portMAX_DELAY);
        }
    }
}

bool CDC_send_data(uint8_t *data_ptr, uint16_t data_size) {
    if (app_cdc_data.handshake_states == APP_CDC_HANDSHAKE_CLIENT_HELLO) {
        // Local copy in a coherent space
        memcpy(app_cdc_data.write_buffer, data_ptr, data_size);
        app_cdc_data.num_bytes_to_write = data_size;
        // Trigger send
        USB_DEVICE_CDC_Write(APP_CDC_DEV_IDX, &app_cdc_data.write_handle, app_cdc_data.write_buffer, data_size, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
        return true;
    }
    return false;
}

/*******************************************************************************
 End of File
 */
