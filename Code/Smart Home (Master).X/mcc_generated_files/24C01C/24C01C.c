#include "24C01C.h"

void EX_EEPROM_Write_Byte(uint8_t reg, uint8_t data){
    I2C_Write1ByteRegister(EEPROM_24C01C_WRITE_ADDRESS, reg, data);
}

uint8_t EX_EEPROM_Read_Byte(uint8_t reg){
    uint8_t received_data = I2C_Read1ByteRegister(EEPROM_24C01C_READ_ADDRESS, reg);
    return received_data;
}