#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "config.h"


# define _XTAL_FREQ 1000000 
# define SWITCH PORTBbits.RB1
# define LED   PORTDbits.RD1


void __interrupt() toggleLed(void);

int main() 
{
    
    // All AN inputs are digital
    ADCON1 = 0x0f;
    
    // RD1 is output (connected to LED)
    TRISD = 0x00;
    
    // RB1 is input (connected to SW0)
    TRISB = 0xff;
    
    
    // Initialize all LEDs to off
    
    
    // Setup all inerrupts
    INTCONbits.GIE = 1; // Globabl interrupt enable
    INTCON3bits.INT1E = 1; // Enable INT1
    INTCON3bits.INT1IF = 0; // reset INT1 flag
    
    INTCON2bits.INTEDG0 = 0; // falling edge
    // INTCON2bits.INTEDG0 = 1; // rising edge
    
    LED = 0; //Initially setting LED = 0. Assuming we are starting with potentiometer is off
    //or potentiometer is all the way to left
    while (1)
    {
        
    }
    return (EXIT_SUCCESS);
}

void __interrupt() toggleLed(void)
{
    // test which interrupt called this Interrupt Service Routine
    
    // INT1
    if (INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCON3bits.INT1IF = 0;
        
        // Do what needs to be done
        if(SWITCH ==1)
        LED = 1;      
        else
        LED = 0;
    }
    return;
}
