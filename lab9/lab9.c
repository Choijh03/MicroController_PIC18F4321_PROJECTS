#include <stdio.h>
#include <stdlib.h>
#include "config.h"



# define _XTAL_FREQ 1000000
void __interrupt() adc_sample(void);

float num;
float new_value;
float temp;
int tens;
int ones;

int main() 
{
      
    // 4 low bit port B to one seg and 4 high bit portB to other seg
    TRISB = 0x00; 

   
  
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // AN0 is connected to wipe of potentiometer
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0E;
    TRISAbits.RA0 = 1;

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0)

    // * Turn on A/D module
    ADCON0bits.ADON = 1;   
    
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
    IPR1bits.ADIP = 1;
    RCONbits.IPEN = 0; // disable priority levels
    INTCONbits.PEIE = 1; // enable peripheral interrupts
    INTCONbits.GIE = 1;
    
    // 3 - Wait the required acquisition time (if required)
    // ---> using ACQT = 2 TAD, no need to manually specify a wait    
 

    //initial setup
    PORTB = 0x00;
    while(1)
    {
        // 4- Start conversion: Set GO/DONE(bar) bit
        ADCON0bits.GO = 1;   
         
       
        // 7 - Go to Step 1 or Step 2 as required                
    }
    
    return 0;

}

void __interrupt() adc_sample(void)
{
    // test which interrupt called this interrupt service routine
    
    // ADC Interrupt
    if (PIR1bits.ADIF && PIE1bits.ADIE)
    {
        // 5 Wait for A/D conversion to complete by either
        // * Polling for the GO/Done bit to be cleared
        // * Waiting for the A/D interrupt
  
        // 6 - Read A/D result registers (ADRESH:ADRESL); clear bit ADIF, if required
        
        // reset the flag to avoid recursive interrupt calls
        PIR1bits.ADIF = 0;
        
       
        
        //num is float
        num = (ADRESH << 8) | ADRESL;  
        
        //temp is float
        temp = (num  * 5 *10);
        
        //new value is float
        new_value =  ( temp  / 1023 ); 
        
        //tens is int
        tens =  (new_value /10 );
        //ones is int
        ones = ( (int)new_value % 10 );
       
        tens = tens << 4;
        
        
        
        PORTB = (tens | ones);
        

    }
}
