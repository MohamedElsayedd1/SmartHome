/* 
 * File:   TC74.h
 * Author: Mohamed Elsayed
 *
 * Created on April 23, 2024, 6:36 AM
 */

#ifndef TC74_H
#define	TC74_H

#include "../../mcc_generated_files/i2c_client/i2c_master_example.h"
#include "../../mcc_generated_files/system/system.h"

#define TC74_TEMPERATURE_READ_ADDRESS  0x9E

uint8_t Read_Temperature(i2c_address_t address);

#endif	/* TC74_H */

