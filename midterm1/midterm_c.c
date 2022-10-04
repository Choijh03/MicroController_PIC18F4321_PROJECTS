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
# define _XTAL_FREQ 1000000 
int main() 
{

    char sseg[6] = {0xCF, 0xE7, 0xF3, 0xF9, 0xBD, 0x9F};
    
    // Turn off ADC
    ADCON1 = 0xff;
    
    // PORTA is output (connected to 7segment display)
    TRISA = 0x00;
    
    // PORTD is input (conntected to switches)
    TRISD = 0xff;
    char counter1 =0;
    char counter2 =5;
    
    while (1)
    {
        if(RD0 == 0 && RD1 ==0)
        {
            PORTA = 0xFF;
        }
        else if(RD0 == 1 && RD1 ==0)
        {
            while(RD0 == 1 && RD1 ==0)
            {
                PORTA = 0xFE;
                __delay_ms(500);
                PORTA = 0xFF;
                __delay_ms(500);     
            }
        }    
        else if(RD0 == 0 && RD1 ==1)
        {
            while(RD0 == 0 && RD1 ==1)
            {
                PORTA = sseg[counter1 & 0x0F];
                __delay_ms(500);
                if(counter1 ==5)
               counter1 = 0; 
                else
               counter1 = counter1+1;
               
            }
        }
        else if(RD0 == 1 && RD1 ==1)
        {
            while(RD0 == 1 && RD1 ==1)
            {
                PORTA = sseg[counter2 & 0x0F];
                __delay_ms(500);
                if(counter2 ==0)
               counter2 = 5; 
                else
               counter2 = counter2-1;
               
            }
            
        }
    }
    return (EXIT_SUCCESS);
}
