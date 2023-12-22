/**
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include "platform.h"
#include "configuration.h"
#include "definitions.h"

#define DEVICE_ADDRESS          0x29

uint8_t RdByte(VL53L5CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_value) {

    if (p_platform->device == VL53L5CX_LEFT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_LEFT_Set(); // Logic 0 disables the I2C comms
    } else if (p_platform->device == VL53L5CX_RIGHT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_RIGHT_Set(); // Logic 0 disables the I2C comms
    }

    uint8_t write_buffer[2];
    write_buffer[1] = RegisterAdress;
    write_buffer[0] = RegisterAdress >> 8;
    bool rtn = DRV_I2C_WriteReadTransfer(p_platform->drvI2CHandle, DEVICE_ADDRESS, write_buffer, 2, p_value, 1);

    uint8_t status = rtn ? 0 : 255;

    return status;
}

uint8_t WrByte(VL53L5CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t value) {

    if (p_platform->device == VL53L5CX_LEFT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_LEFT_Set(); // Logic 0 disables the I2C comms
    } else if (p_platform->device == VL53L5CX_RIGHT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_RIGHT_Set(); // Logic 0 disables the I2C comms
    }

    uint8_t write_buffer[3];
    write_buffer[1] = RegisterAdress;
    write_buffer[0] = RegisterAdress >> 8;
    write_buffer[2] = value;
    bool rtn = DRV_I2C_WriteTransfer(p_platform->drvI2CHandle, DEVICE_ADDRESS, &write_buffer, 3);

    uint8_t status = rtn ? 0 : 255;

    return status;
}

uint8_t WrMulti(VL53L5CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {

    if (p_platform->device == VL53L5CX_LEFT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_LEFT_Set(); // Logic 0 disables the I2C comms
    } else if (p_platform->device == VL53L5CX_RIGHT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_RIGHT_Set(); // Logic 0 disables the I2C comms
    }

    uint8_t *write_buffer;

    write_buffer = OSAL_Malloc(size + 2);
    if (write_buffer != NULL) {
        write_buffer[1] = RegisterAdress;
        write_buffer[0] = RegisterAdress >> 8;
        for (uint32_t i = 0; i < size; i++) {
            write_buffer[i + 2] = p_values[i];
        }
        bool rtn = DRV_I2C_WriteTransfer(p_platform->drvI2CHandle, DEVICE_ADDRESS, &write_buffer, size + 2);
        OSAL_Free(write_buffer);
        
        uint8_t status = rtn ? 0 : 255;

        return status;
    }
    return 255u;
}

uint8_t RdMulti(VL53L5CX_Platform *p_platform, uint16_t RegisterAdress, uint8_t *p_values, uint32_t size) {

    if (p_platform->device == VL53L5CX_LEFT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_LEFT_Set(); // Logic 0 disables the I2C comms
    } else if (p_platform->device == VL53L5CX_RIGHT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_RIGHT_Set(); // Logic 0 disables the I2C comms
    }

    uint8_t write_buffer[2];
    write_buffer[1] = RegisterAdress;
    write_buffer[0] = RegisterAdress >> 8;
    bool rtn = DRV_I2C_WriteReadTransfer(p_platform->drvI2CHandle, DEVICE_ADDRESS, write_buffer, 2, p_values, size);

    uint8_t status = rtn ? 0 : 255;


    return status;
}

uint8_t Reset_Sensor(VL53L5CX_Platform *p_platform) {
    uint8_t status = 0;

    // Reset both chips
    TOF_RST_RIGHT_Clear();
    TOF_RST_LEFT_Clear();
    WaitMs(p_platform, 10);
    TOF_RST_RIGHT_Set();
    TOF_RST_LEFT_Set();
    WaitMs(p_platform, 10);
    TOF_RST_RIGHT_Clear();
    TOF_RST_LEFT_Clear();

    if (p_platform->device == VL53L5CX_LEFT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_RIGHT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_LEFT_Set(); // Logic 0 disables the I2C comms
        /* Set opposite device pin RST to LOW */
        //TOF_RST_RIGHT_Set(); // Logic 1 keep in reset

        //        /* Set pin LPN to LOW */
        //        TOF_LPn_LEFT_Clear();
        //
        //        /* Set pin AVDD to LOW */
        //        /* Set pin VDDIO  to LOW */
        //        WaitMs(p_platform, 100);
        //
        //        /* Set pin LPN of to HIGH */
        //        TOF_LPn_LEFT_Set();
        //
        //        /* Set pin AVDD of to HIGH */
        //        /* Set pin VDDIO of  to HIGH */
        //        WaitMs(p_platform, 100);
    } else if (p_platform->device == VL53L5CX_RIGHT) {
        /* Set opposite device pin LPN to LOW */
        TOF_LPn_LEFT_Clear(); // Logic 0 disables the I2C comms
        TOF_LPn_RIGHT_Set(); // Logic 0 disables the I2C comms
        /* Set opposite device pin RST to LOW */
        //TOF_RST_LEFT_Set(); // Logic 1 keep in reset

        //        /* Set pin LPN to LOW */
        //        TOF_LPn_RIGHT_Clear();
        //
        //        /* Set pin AVDD to LOW */
        //        /* Set pin VDDIO  to LOW */
        //        WaitMs(p_platform, 100);
        //
        //        /* Set pin LPN of to HIGH */
        //        TOF_LPn_RIGHT_Set();
        //
        //        /* Set pin AVDD of to HIGH */
        //        /* Set pin VDDIO of  to HIGH */
        //        WaitMs(p_platform, 100);
    } else {
        status = 255;
    }
    return status;
}

void SwapBuffer(uint8_t *buffer, uint16_t size) {
    uint32_t i, tmp;

    /* Example of possible implementation using <string.h> */
    for (i = 0; i < size; i = i + 4) {
        tmp = (
                buffer[i] << 24)
                | (buffer[i + 1] << 16)
                | (buffer[i + 2] << 8)
                | (buffer[i + 3]);

        memcpy(&(buffer[i]), &tmp, 4);
    }
}

uint8_t WaitMs(VL53L5CX_Platform *p_platform, uint32_t TimeMs) {
    uint8_t status = 255;

    vTaskDelay(TimeMs / portTICK_PERIOD_MS);

    status = 0;
    return status;
}
