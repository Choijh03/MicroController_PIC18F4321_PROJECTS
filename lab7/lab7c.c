/* 
 * File:   newmain.c
 * Author: PC
 *
 * Created on 2022? 7? 4? (?), ?? 6:22
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

/*
 * 
 */

#define LED PORTDbits.RD1
#define SWITCH PORTBbits.RB1
int main() 
{
    // Turn off the ADC
    ADCON1 = 0xff;
    
    //; PORTB, bit1-input
    //; PORTD, bit1-output
    TRISB = 0xFF;  
    TRISD = 0x00;
    
    while(1)
    {
       if(SWITCH ==1)
           LED = 1;
       else
           LED = 0;
    }
    
    return (EXIT_SUCCESS);
}
