#include "DS1307.h"

static DS1307_t DS1307_obj;

static uint8_t Get_seconds(i2c_address_t address, uint8_t reg);
static uint8_t Get_minutes(i2c_address_t address, uint8_t reg);
static uint8_t Get_hours(i2c_address_t address, uint8_t reg);
static uint8_t Get_years(i2c_address_t address, uint8_t reg);
static uint8_t Get_months(i2c_address_t address, uint8_t reg);
static uint8_t Get_days(i2c_address_t address, uint8_t reg);


DS1307_t Get_time_And_Date(void){
    DS1307_obj._Second = Get_seconds(0xD0, 0x00);
    DS1307_obj._Minute = Get_minutes(0xD0, 0x01);
    DS1307_obj._Hour = Get_hours(0xD0, 0x02);
    DS1307_obj._Year = Get_years(0xD0, 0x06);
    DS1307_obj._Month = Get_months(0xD0, 0x05);
    DS1307_obj._Day = Get_days(0xD0, 0x04);
    return DS1307_obj;
}

static uint8_t Get_seconds(i2c_address_t address, uint8_t reg){
    uint8_t seconds = I2C_Read1ByteRegister(address, reg);
    return seconds;
}

static uint8_t Get_minutes(i2c_address_t address, uint8_t reg){
    uint8_t minutes = I2C_Read1ByteRegister(address, reg);
    return minutes;
}

static uint8_t Get_hours(i2c_address_t address, uint8_t reg){
    uint8_t hours = I2C_Read1ByteRegister(address, reg);
    return hours;
}

static uint8_t Get_years(i2c_address_t address, uint8_t reg){
    uint8_t years = I2C_Read1ByteRegister(address, reg);
    return years;
}

static uint8_t Get_months(i2c_address_t address, uint8_t reg){
    uint8_t months = I2C_Read1ByteRegister(address, reg);
    return months;
}

static uint8_t Get_days(i2c_address_t address, uint8_t reg){
    uint8_t days = I2C_Read1ByteRegister(address, reg);
    return days;
}