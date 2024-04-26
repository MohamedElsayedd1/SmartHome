/**
 * @file nvm.c
 *
 * @ingroup nvm_driver
 *
 * @brief Contains the implementation file for the NVM driver.
 *
 * @version NVM Driver Version 1.0.1 
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

#include "../nvm.h"

/*
 * AoU: These variables are placed in the Access RAM using the "__near" qualifier.
 *      Additionally, "Address Qualifiers" must be configured to the "Require" under
 *      "Project Properties -> XC8 Compiler -> Optimizations".
 */
__near volatile static uint8_t unlockKeyLow;
__near volatile static uint8_t unlockKeyHigh;

void NVM_Initialize(void)
{
    NVM_StatusClear();
}

bool NVM_IsBusy(void)
{
    return (EECON1bits.WR || EECON1bits.RD);
}

nvm_status_t NVM_StatusGet(void)
{
    if (EECON1bits.WRERR == 1)
    {
        return NVM_ERROR;
    }
    else
    {
        return NVM_OK;
    }
}

void NVM_StatusClear(void)
{
    EECON1bits.WRERR = 0;
}

void NVM_UnlockKeySet(uint16_t unlockKey)
{
    unlockKeyHigh = (uint8_t) (unlockKey >> 8);
    unlockKeyLow = (uint8_t) unlockKey;
}

void NVM_UnlockKeyClear(void)
{
    unlockKeyHigh = 0x00;
    unlockKeyLow = 0x00;
}

flash_data_t FLASH_Read(flash_address_t address)
{
    //Save the table pointer
    uint32_t tablePointer = ((uint32_t) TBLPTRU << 16) | ((uint32_t) TBLPTRH << 8) | ((uint32_t) TBLPTRL);
    
    //Load table pointer with the target address of the byte  
    TBLPTRU = (uint8_t) (address >> 16);
    TBLPTRH = (uint8_t) (address >> 8);
    TBLPTRL = (uint8_t) address;

    asm("TBLRD");
    
    //Restore the table pointer
    TBLPTRU = (uint8_t) (tablePointer >> 16);
    TBLPTRH = (uint8_t) (tablePointer >> 8);
    TBLPTRL = (uint8_t) tablePointer;
    
    return (TABLAT);
}

nvm_status_t FLASH_RowWrite(flash_address_t address, flash_data_t *dataBuffer)
{    
    uint8_t flashDataCount = PROGMEM_PAGE_SIZE;

     //Save the table pointer
    uint32_t tablePointer = ((uint32_t) TBLPTRU << 16) | ((uint32_t) TBLPTRH << 8) | ((uint32_t) TBLPTRL);
    
    //Save global interrupt enable bit value
    uint8_t globalInterruptBitValue = INTCONbits.GIE;   

    //Access program Flash memory
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 1;   
    
    //Enable write operation
    EECON1bits.WREN = 1;     
    
    //Load table pointer with the target address of the byte  
    TBLPTRU = (uint8_t) (address >> 16);
    TBLPTRH = (uint8_t) (address >> 8);
    TBLPTRL = (uint8_t) address;

    while (flashDataCount-- > 0U)
    {
         TABLAT = (uint8_t)*dataBuffer;
        
        dataBuffer++;
        
        //If last latch to be written
        if (flashDataCount == 0U)
        {
            //Write program Flash memory 
            asm("TBLWT");
        }
        else
        {
            asm("TBLWTPOSTINC");
        }   
        //Disable global interrupt
        INTCONbits.GIE = 0;    
        
        //Perform the unlock sequence
        EECON2 = unlockKeyLow;
        EECON2 = unlockKeyHigh;
        EECON1bits.WR = 1;
        NOP();
        NOP();
            
        //Restore global interrupt enable bit value
        INTCONbits.GIE = globalInterruptBitValue;        
    }
    
    //Disable write operation
    EECON1bits.WREN = 0;      
    
    if (EECON1bits.WRERR == 1) 
    {
        return NVM_ERROR;
    } 
    else 
    {
        return NVM_OK;
    }
}

nvm_status_t FLASH_PageErase(flash_address_t address)
{
    //Save the table pointer
    uint32_t tablePointer = ((uint32_t) TBLPTRU << 16) | ((uint32_t) TBLPTRH << 8) | ((uint32_t) TBLPTRL);
    
    //Save global interrupt enable bit value
    uint8_t globalInterruptBitValue = INTCONbits.GIE;   
    
    //Load table pointer with the target address of the byte  
    TBLPTRU = (uint8_t) (address >> 16);
    TBLPTRH = (uint8_t) (address >> 8);
    TBLPTRL = (uint8_t) address;

    //Access program Flash memory EEPGD
    EECON1bits.EEPGD = 1;
    
    //Access Flash program or data EEPROM memory CFGS
    EECON1bits.CFGS = 0;

    //Allows write cycles to Flash program WREN
    EECON1bits.WREN = 1;

    //Erase the program memory row addressed by TBLPTR with the next WR command FREE
    EECON1bits.FREE = 1;     

    //Disable global interrupt
    INTCONbits.GIE = 0;
    
    //Perform the unlock sequence
    EECON2 = unlockKeyLow;
    EECON2 = unlockKeyHigh;
    
    //Restore global interrupt enable bit value
    INTCONbits.GIE = globalInterruptBitValue;	

    //Inhibits write cycles to Flash program WREN
    EECON1bits.WREN = 0;      
    
    //Restore the table pointer
    TBLPTRU = (uint8_t) (tablePointer >> 16);
    TBLPTRH = (uint8_t) (tablePointer >> 8);
    TBLPTRL = (uint8_t) tablePointer;
    
    if (EECON1bits.WRERR == 1) 
    {
       return NVM_ERROR;
    }
    else
    {
        return NVM_OK;
    }
}

flash_address_t FLASH_PageAddressGet(flash_address_t address)
{
    return (flash_address_t) (address & ((PROGMEM_SIZE - 1U) ^ (PROGMEM_PAGE_SIZE - 1U)));
}

uint16_t FLASH_PageOffsetGet(flash_address_t address)
{
    return (uint16_t) (address & (PROGMEM_PAGE_SIZE - 1U));
}

eeprom_data_t EEPROM_Read(eeprom_address_t address)
{
    //Access EEPROM
    EECON1bits.CFGS = 0;
    EECON1bits.EEPGD = 0;  
    
    //Load EEADR with the EEPROM address
    EEADR = (uint8_t) address;
    
    //Initiate Read
    EECON1bits.RD = 1;
    NOP();  // NOPs may be required for latency at high frequencies
    NOP();

    return EEDATA;
}

void EEPROM_Write(eeprom_address_t address, eeprom_data_t data)
{
    // Wait for any previous write operation to complete
    while (EECON1bits.WR)
        continue;
    
    // Set the address to write to
    EEADR = address + 0x00;
    
    // Load the data to write
    EEDATA = data;
    
    // Clear the write-enable latch
    EECON1bits.WREN = 1;
    
    // Disable interrupts
    INTCONbits.GIE = 0;
    
    // Start the write sequence
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    
    // Wait for write to complete
    while (EECON1bits.WR)
        continue;
    
    // Enable interrupts
    INTCONbits.GIE = 1;
    
    // Clear the write-enable latch
    EECON1bits.WREN = 0;
}
