
#include "mcc_generated_files/system/system.h"

/*
    Main application
*/

uint8_t Temp = 0;

void My_I2C_SlaveSetReadIntHandler(i2cInterruptHandler handler){
    Temp = SSPBUF;
}

int main(void)
{
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts 
    // Use the following macros to: 

    // Enable the Global Interrupts 
    INTERRUPT_GlobalInterruptEnable();  
    // Enable the Peripheral Interrupts 
    INTERRUPT_PeripheralInterruptEnable(); 
    
    I2C_Open();
    I2C_SlaveSetReadIntHandler(My_I2C_SlaveSetReadIntHandler);

    while(1)
    {
        
        if(Temp >= 30){
            TRISDbits.RD0 = 0;
            LATDbits.LATD0 = 1;
        }
        else if(Temp < 30){
            LATDbits.LATD0 = 0;
        }
    }    
}