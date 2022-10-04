/* 
 * File:   newmain.c
 * Author: PC
 *
 * Created on 2022? 7? 4? (?), ?? 8:07
 */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"
/*
 * 
 */
int main() 
{

    char sseg[16] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};
    
    // Turn off ADC
    ADCON1 = 0xff;
    
    // PORTA is output (connected to 7segment display)
    TRISA = 0x00;
    
    // PORTB is input (conntected to switches)
    TRISB = 0xff;
    
    while (1)
    {
        PORTA = sseg[PORTB & 0x0F];
    }
    return (EXIT_SUCCESS);
}
