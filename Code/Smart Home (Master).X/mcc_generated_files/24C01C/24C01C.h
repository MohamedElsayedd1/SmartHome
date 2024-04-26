/* 
 * File:   24C01C.h
 * Author: Dubai Store
 *
 * Created on April 23, 2024, 2:54 AM
 */

#ifndef M24C01C_H
#define	M24C01C_H

#include "../../mcc_generated_files/i2c_client/i2c_master_example.h"

#define EEPROM_24C01C_WRITE_ADDRESS 0xA2
#define EEPROM_24C01C_READ_ADDRESS  0xA3

void EX_EEPROM_Write_Byte(uint8_t reg, uint8_t data);
uint8_t EX_EEPROM_Read_Byte(uint8_t reg);

#endif	/* 24C01C_H */

