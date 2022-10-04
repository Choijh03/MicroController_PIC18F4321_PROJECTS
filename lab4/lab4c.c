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

#define LEDC PORTCbits.RC0
#define LEDD PORTDbits.RD0
#define SWITCHC PORTCbits.RC1
#define SWITCHD PORTDbits.RD1
int main() 
{
          // Turn off the ADC
    ADCON1 = 0xff;
    
    //; PORTC, bit1-input, bit0-output
	//; PORTD, bit1-input, bit0-output
    TRISC = 0x02;  
    TRISD = 0x02;
    // If input in C is high & input in D is low, LED at D will be on
	// If input in C is low & input in D is high, LED at C will be on
	// If input in C is high & input in D is high, LED off
    // If input in C is low & input in D is low, LED both on
	// LED turns on whenever is low
    
    while(1)
    {
        LEDC = SWITCHC; //cathode is connected
        LEDD = SWITCHD; //cathode is connected
    }
    
    return (EXIT_SUCCESS);
}
