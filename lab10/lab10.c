#include <stdio.h>
#include <stdlib.h>
#include "config.h"
#include "LiquidCrystal.h"

# define _XTAL_FREQ 1000000 
#define up PORTBbits.RB0 
#define down PORTBbits.RB1
#define reset PORTBbits.RB2
int main() 
{
    
    // ALL ANx input are digital
    ADCON1 = 0x0f;
    
    // Data pins connected to PORTA
    TRISA = 0x00; 
    
    // RS = RD0
    // RW = RD1
    // E  = RD2
    TRISD = 0x00;
    
    TRISB = 0x07;
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTA, &PORTD);
    
    // initialize the LCD to be 16x2 (this is what I have, yours might be different)
    begin(16, 2, LCD_5x8DOTS);
    
    int num = 0;   
    while (1)
    {
        
        if(up ==0 )
        {
            clear();
            num = num +1;
            print_int(num);
            __delay_ms(100);
        }
        else if(down ==0 )
        {
            clear();
            num = num -1;
            print_int(num);
            __delay_ms(100);
        }    
        else if(reset ==0 )     
        {
            clear();
            num = 0;
            print_int(num);
            __delay_ms(100);
        }
        else
        {
            clear();
            print_int(num);
            __delay_ms(100);
        }
     
        
    }
    return 0;

}

