#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"


# define _XTAL_FREQ 1000000
# define left PORTDbits.RD4
# define right PORTDbits.RD3

void __interrupt() adc_sample(void);


volatile int num_pot1;
volatile int num_pot2;
volatile int result_addition;
volatile int result_subtraction;
long long result_multiplication;


int main() 
{
    
   
    TRISB = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0b00011000;
    

    pin_setup(&PORTB, &PORTD);
    
    
    begin(16, 2, LCD_5x8DOTS);
    
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module

    // * Configure analog pins, voltage reference and digital I/O 
    // Reference voltages are VSS and VDD
    ADCON1 = 0x0D;
    TRISAbits.RA0 = 1; // connected to potentiometer1
    TRISAbits.RA1 = 1; // connected to potentiometer2

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 1; // Right justified

    // * Select A/D input channel
    ADCON0bits.CHS = 0; // Channel 0 (AN0), starting with the potentiometer

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
 
    while(1)
    {  
        // 4- Start conversion: Set GO/DONE(bar) bit
        ADCON0bits.GO = 1;
         
        
        if(left == 0 && right ==0)
         { 
            noDisplay();
         } 
        else if(left==0 && right ==1)
        {   
           
            display();        
            home();
            print("(");
            print_int(num_pot1);
            print(")+");
            print("(");
            print_int(num_pot2);
            print(")=");
            print("             ");
            setCursor(0,1);
            result_addition = 0;
            result_addition = num_pot1 + num_pot2;
            print_int(result_addition);
            print("             ");
        }
        else if(left==1 && right ==0)
        {   
           
            display();        
            home();
            print("(");
            print_int(num_pot1);
            print(")-");
            print("(");
            print_int(num_pot2);
            print(")=");
             print("             ");
            setCursor(0,1);
            result_subtraction = 0;
            result_subtraction = num_pot1 - num_pot2;
            print_int(result_subtraction);
            print("             ");
        } 
        else if(left==1 && right ==1)
        {   
            
            display();        
            home();
            print("(");
            print_int(num_pot1);
            print(")x");
            print("(");
            print_int(num_pot2);
            print(")= ");
            print("             ");
            setCursor(0,1);  
            result_multiplication = (long long)num_pot1 * (long long)num_pot2;
            print_longint(result_multiplication);           
            print("             ");                              
        }   
                
           

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
        
        if (ADCON0bits.CHS == 0) // channel AN0 (potentiometer1)
        {
            num_pot1 = (ADRESH << 8) | ADRESL;
            //mid point is about 511.5
            if(num_pot1 < 512)
                num_pot1 = num_pot1 - 511;
            else
                num_pot1 = num_pot1 - 512;
            
            
            
            ADCON0bits.CHS = 1;
            
         
            
        }
        else if (ADCON0bits.CHS == 1) // channel AN1 (potentiometer2)
        {
            num_pot2 = (ADRESH << 8) | ADRESL;
            if(num_pot2 < 512)
                num_pot2 = num_pot2 - 511;
            else
                num_pot2 = num_pot2 - 512;
            ADCON0bits.CHS = 0;
            
            
        }
        

    }
}

