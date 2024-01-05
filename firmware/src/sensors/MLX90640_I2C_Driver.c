/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "MLX90640_I2C_Driver.h"


MLX90640_I2C_DATA mlx90640_i2c_data;

void MLX90640_I2CInit(DRV_HANDLE drvI2CHandle) {
    mlx90640_i2c_data.drvI2CHandle = drvI2CHandle;
}

int MLX90640_I2CGeneralReset(void) {
    // General I2C reset, send 0x06 to 0x00 address
    uint8_t data = 0x06;
    DRV_I2C_WriteTransfer(mlx90640_i2c_data.drvI2CHandle, 0x00, &data, 1);

    return 0;
}

int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *p_data) {

    uint8_t write_buffer[2];
    write_buffer[1] = startAddress;
    write_buffer[0] = startAddress >> 8;
    bool rtn = DRV_I2C_WriteReadTransfer(mlx90640_i2c_data.drvI2CHandle, slaveAddr, write_buffer, 2, p_data, nMemAddressRead * 2);

    uint8_t *ptr = (uint8_t *) p_data;
    for (uint16_t i = 0; i < nMemAddressRead; i++) {
        uint8_t temp = ptr[i * 2];
        ptr[i * 2] = 0xFF & ptr[i * 2 + 1];
        ptr[i * 2 + 1] = 0xFF & temp;
    }

    return rtn ? 0 : -1;
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data) {
    uint8_t reg_addr[2];
    reg_addr[1] = writeAddress;
    reg_addr[0] = writeAddress >> 8;

    uint8_t reg_data[2];
    reg_data[1] = data;
    reg_data[0] = data >> 8;

    bool rtn = DRV_I2C_WriteTransferPlusAddress(mlx90640_i2c_data.drvI2CHandle, slaveAddr, reg_addr, 2, reg_data, 2);

    return rtn ? 0 : -1;
}

//void MLX90640_I2CFreqSet(int freq) {
//
//}
