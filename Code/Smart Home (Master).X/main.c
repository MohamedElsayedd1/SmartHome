 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
*/

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include "mcc_generated_files/system/system.h"
#include "mcc_generated_files/DS1307/DS1307.h"
#include "mcc_generated_files/24C01C/24C01C.h"
#include "mcc_generated_files/TC74/TC74.h"

/*
    Main application
*/

#define MCU_SLAVE_ADDRESS 0X70


uint8_t hexToDecimal(uint8_t hex[]);
void convert_byte_to_str(uint8_t *str, uint8_t val);
void convert_decimal_to_str(uint8_t *str, uint8_t val);
void EUSART_Send_Date(void);
void EUSART_Send_Time(void);
void EUSART_Send_Temp(void);

DS1307_t DS1307_obj;
uint8_t dummy_str[2];
uint8_t hex_str[2];
uint8_t received_data = 0;
uint8_t TC74_Temp = 0;
uint8_t decimal = 0;

int main(void)
{
    SYSTEM_Initialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptHighEnable(); 
    INTERRUPT_GlobalInterruptLowEnable(); 
    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 

    // Disable the Peripheral Interrupts 
    //INTERRUPT_PeripheralInterruptDisable();
    

    /*
    EUSART_Send_String("System is opened \r");
    __delay_ms(500);

    
    EX_EEPROM_Write_Byte(0x00, 0x06);
    __delay_ms(100);
    received_data = EX_EEPROM_Read_Byte(0x03);
    */
        
    while(1){
        DS1307_obj = Get_time_And_Date();
        TC74_Temp = Read_Temperature(0x9A);
        EUSART_Send_Date();
        EUSART_Send_Time(); 
        EUSART_Send_Temp();
        I2C_Write1Byte(MCU_SLAVE_ADDRESS, TC74_Temp);
    }
}


void EUSART_Send_Date(void){
    EUSART_Send_String("Date : ");
    convert_byte_to_str(dummy_str, DS1307_obj._Day);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write('/');
    convert_byte_to_str(dummy_str, DS1307_obj._Month);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write('/');
    convert_byte_to_str(dummy_str, DS1307_obj._Year);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write('\r');
    __delay_ms(250);
}

void EUSART_Send_Time(void){
    EUSART_Send_String("Time : ");
    convert_byte_to_str(dummy_str, DS1307_obj._Hour);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write(':');
    convert_byte_to_str(dummy_str, DS1307_obj._Minute);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write(':');
    convert_byte_to_str(dummy_str, DS1307_obj._Second);
    EUSART_Send_String(dummy_str);
    while(!EUSART_IsTxReady());
    EUSART_Write('\r');
    __delay_ms(250);
}

void EUSART_Send_Temp(void){
    EUSART_Send_String("Temperature : ");
    convert_byte_to_str(hex_str, TC74_Temp);
    EUSART_Send_String(hex_str);
    while(!EUSART_IsTxReady());
    EUSART_Write('C');
    EUSART_Send_String(" (in hexa) ");
    while(!EUSART_IsTxReady());
    EUSART_Write('\r');
}

void convert_byte_to_str(uint8_t *str, uint8_t val){
    memset(str, '\0', 3);
    sprintf(str, "%x", val);
}

void convert_decimal_to_str(uint8_t *str, uint8_t val){
    memset(str, '\0', 3);
    sprintf(str, "%d", val);
}



