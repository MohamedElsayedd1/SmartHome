/* 
 * File:   TC74.h
 * Author: Mohamed Elsayed
 *
 * Created on April 23, 2024, 6:36 AM
 */

#include "TC74.h"

uint8_t Read_Temperature(i2c_address_t address){
    uint8_t temp = 0;
    uint8_t temp_data = 0;
    temp = I2C_Read1ByteRegister(address, 0x00);
    temp_data = SSPBUF;
    return temp;
}
