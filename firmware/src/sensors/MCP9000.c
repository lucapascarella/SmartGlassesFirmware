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

#include "MCP9000.h"
#include "configuration.h"

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

MCP9800_DATA mcpData;

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

void MCP9800_Initialize(void) {

    MCP9800_REG_POINTER reg_pointer;
    MCP9800_REG_CONFIG config = {0};
    MCP9800_REG_AMBIENT temperature = {0};

    DRV_HANDLE drvI2CHandle = DRV_HANDLE_INVALID;
    drvI2CHandle = DRV_I2C_Open(DRV_I2C_INDEX_0, DRV_IO_INTENT_READWRITE);
    if (drvI2CHandle != DRV_HANDLE_INVALID) {

        reg_pointer = MCP9800_CONFIG;
        config.bits.one_shot = true;
        config.bits.resolution = 0b11u;
        uint8_t tmp[2];
        tmp[0] = reg_pointer;
        tmp[1] = config.bytes.lsb;

        if (DRV_I2C_WriteTransfer(drvI2CHandle, MCP9800_BASE_ADDR, tmp, 2)) {
            logDebug("MCP9800 write config\r\n");
            config.val = 0;
            if (DRV_I2C_WriteReadTransfer(drvI2CHandle, MCP9800_BASE_ADDR, (uint8_t*) & reg_pointer, 1, (uint8_t*) & config, 1)) {
                logDebug("MCP9800 resolution %d\r\n", config.bits.resolution);
                reg_pointer = MCP9800_AMBIENT;
                if (DRV_I2C_WriteReadTransfer(drvI2CHandle, MCP9800_BASE_ADDR, (uint8_t*) & reg_pointer, 1, (uint8_t*) & temperature, 2)) {
                    float temp = temperature.bytes.lsb + temperature.bytes.msb / 10.0;
                    logDebug("Temperature %d.%d C, %.2f C\r\n", temperature.bytes.lsb, temperature.bytes.msb, temp);
                } else {
                    logFatal("Cannot read I2C_DRV\r\n");
                }
            } else {
                logFatal("Cannot write/read I2C_DRV\r\n");
            }
        } else {
            logFatal("Cannot write I2C_DRV\r\n");
        }
    } else {
        logFatal("Cannot open I2C_DRV\r\n");
    }

    //    MCP9800_REG_POINTER reg_pointer;
    //    MCP9800_REG_CONFIG config = {0};
    //
    //    while (I2C1_IsBusy()) {
    //        vTaskDelay(1U / portTICK_PERIOD_MS);
    //    }
    //
    //    reg_pointer = MCP9800_CONFIG;
    //    config.bits.one_shot = true;
    //    config.bits.resolution = 0b11u;
    //    uint8_t tmp[2];
    //    tmp[0] = reg_pointer;
    //    tmp[1] = config.bytes.lsb;
    //    bool rtn = I2C1_Write(MCP9800_BASE_ADDR, tmp, 2);
    //
    //    while (I2C1_IsBusy()) {
    //        vTaskDelay(1U / portTICK_PERIOD_MS);
    //    }
    //    I2C_ERROR error = I2C1_ErrorGet();
    //
    //    memset(&config, 0xFF, sizeof (config));
    //    rtn = I2C1_WriteRead(MCP9800_BASE_ADDR, (uint8_t*) & reg_pointer, 1, (uint8_t*) & config, 1);
    //    while (I2C1_IsBusy()) {
    //        vTaskDelay(1U / portTICK_PERIOD_MS);
    //    }
    //    error = I2C1_ErrorGet();
    //
    //    vTaskDelay(1U / portTICK_PERIOD_MS);
    //    MCP9800_REG_AMBIENT temperature = {0};
    //    reg_pointer = MCP9800_AMBIENT;
    //    rtn = I2C1_WriteRead(MCP9800_BASE_ADDR, (uint8_t*) & reg_pointer, 1, (uint8_t*) & temperature, 2);
    //    if (rtn) {
    //        while (I2C1_IsBusy()) {
    //            vTaskDelay(1U / portTICK_PERIOD_MS);
    //        }
    //        error = I2C1_ErrorGet();
    //        //        float temp = 
    //        logDebug("Temperature %d.%d C\r\n", temperature.bytes.lsb, temperature.bytes.msb);
    //    } else {
    //        logFatal("Cannot read temperature\r\n");
    //    }


}


/*******************************************************************************
 End of File
 */
