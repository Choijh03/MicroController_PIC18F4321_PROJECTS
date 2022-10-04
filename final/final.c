#include <stdio.h>
#include <stdlib.h>
#include "LiquidCrystal.h"
#include "config.h"

#define _XTAL_FREQ 1000000
#define redLED PORTBbits.RB1

void __interrupt(high_priority) stop_program(void);
void __interrupt(low_priority) two_Interrupts(void);

int button_count =0;
int max_speed = 0;
int delay_in = 0;
int duty_cycle = 0;
int remainder = 0;
int interval_size = 0;
int remainder_temp = 0;


int main() 
{

    TRISD = 0x00;
    //LCD data pins connected
    
    TRISE = 0x00; //RE is output  
    // RS = RE0
    // RW = RE1
    // E  = RE2
    
    // connect the LCD pins to the appropriate PORT pins
    pin_setup(&PORTD, &PORTE);
    
    // initialize the LCD to be 16x2
    begin(16, 2, LCD_5x8DOTS);
    
    //--------------------------------------------------------------------------
    // 1 - Configure the A/D Module
    ADCON1 = 0x0D;
    TRISAbits.RA0 = 1; // connected to potentiometer1
    TRISAbits.RA1 = 1; // connected to potentiometer2

    // * Select A/D acquisition time
    // * Select A/D conversion clock
    // Right justified, ACQT = 2 TAD, ADCS = FOSC/2
    ADCON2bits.ADCS = 0; // FOSC/2
    ADCON2bits.ACQT = 1; // ACQT = 2 TAD
    ADCON2bits.ADFM = 0; // Right justified
    
     // * Select A/D input channel
    ADCON0bits.CHS = 0;  // Channel 0 (AN0), starting with the potentiometer
    
    // * Turn on A/D module
    ADCON0bits.ADON = 1;
    
    // 2 - Configure A/D interrupt (if desired)
    // * Clear ADIF bit
    // * Set ADIE bit
    // * Select interrupt priority ADIP bit
    // * Set GIE bit
    
    PIR1bits.ADIF = 0; 
    PIE1bits.ADIE = 1; 
    IPR1bits.ADIP = 0; 
    RCONbits.IPEN = 1; // enable priority levels
    INTCONbits.PEIE = 1; // enable peripheral peripheral
    INTCONbits.GIE = 1; 
    
    //---------------------interrupt0 setup----------------------------------
    INTCONbits.INT0E = 1; // Enable INT0
    INTCONbits.INT0IF = 0; // reset INT0 flag
    INTCON2bits.INTEDG0 = 0; // falling edge
    // INTCON2bits.INTEDG0 = 1; // rising edge
    //-----------------------------------------------------------------------
    
    //------------------ setup timer0----------------------------------------- 
    T0CONbits.PSA = 1; // Prescaler is assigned
    //T0CONbits.T0PS = 0x05; // 1:64 prescale value
    T0CONbits.T0CS = 0; // clock source is internal instruction cycle
    T0CONbits.T08BIT = 0; // operate in 16 bit mode nowt
    T0CONbits.TMR0ON = 1; // Turn on timer
    TMR0 = 63100; // initial timer value to get to the 10 ms delay

    // setting up timer0 as an interrupt
    INTCONbits.TMR0IE = 1; 
    INTCONbits.TMR0IF = 0; 
    INTCON2bits.TMR0IP = 0; //Low priority
    //-----------------------------------------------------------------------

    //-----------------------PWM setup---------------------------------
    // PWM Frequency = 1 KHz
    // PWM Period = 1x10^-3 Second
    PR2 = 249;
    
    // setup timer 2
    // FOSC = 1 MHz
    T2CONbits.T2CKPS = 0b00;
    T2CONbits.TMR2ON = 1;
    
     // setup CCP2 in PWM mode
    TRISCbits.RC1 = 0;
    CCP2CONbits.CCP2M = 0b1100;
    //-----------------------------------------------------------------------
    
    
    // Initial LED states
    TRISBbits.RB1 = 0;   //Green LED off
    redLED = 0;   //Red LED off

    while(1)
    {
        // 4- Start conversion: Set GO/DONE(bar) bit
        ADCON0bits.GO = 1;
        display();
        home(); //first row
        print("Max Speed: ");
        print_int(max_speed);
        print("             ");
        setCursor(0, 1); //second row
        print("Delay: ");
        print_int(delay_in * 10);
        print(" mS");
        print("             ");
        
        //when max_speed > delay_in
        interval_size = (max_speed / delay_in);  
        //ex: 800 / 500 =1, remainder 300 will be calculated in remainder_temp
        remainder_temp = max_speed - ( (max_speed / delay_in) * delay_in ) ;
        //ex: 800 - (800/ 500) *500 = 300
        //if max_speed is smaller than delay_in, 
        //duty_cycle_temp = 0 and interval_size = max_speed
        
        // 7 - Go to Step 1 or Step 2 as required         
               
    }
    return 0;
}

//high interrupt
void __interrupt(high_priority) stop_program(void) 
{
    // INT0
    if (INTCONbits.INT0IE && INTCONbits.INT0IF) 
    {
        button_count++;
        if(button_count>2)
        {button_count=0;}
        // Turn off the interrupt flag to avoid recursive interrupt calls
        INTCONbits.INT0IF = 0;
        
        
        TRISC = ~TRISC; //toggling CCP2 or RC1
        redLED = 0;
        clear(); //to clear second row
        
        
        while (INTCONbits.INT0IF == 0) 
        {
             
            if(button_count ==1)
             {
                 home();
                 print("    STOPPED!      ");
                 print("             ");
                 redLED = 1;
                 TRISBbits.RB1 = 0;
                    
             }                
             else 
             {                 
                 redLED =0;      
                 __delay_ms(300);
                 return;
             }                                                   
        }     
        __delay_ms(300);
        return;
    }
}


//low interrupt
void __interrupt(low_priority) two_Interrupts(void) 
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
        
        //max_speed AN0
        if (ADCON0bits.CHS == 0) 
        {           
            max_speed = (ADRESH << 2) | (ADRESL >> 6);
            ADCON0bits.CHS = 1;
        }
        //delay AN1
        else if(ADCON0bits.CHS == 1) 
        {
            delay_in = (ADRESH << 2) | (ADRESL >> 6);
            ADCON0bits.CHS = 0;
        }
    }
    
    if (INTCONbits.TMR0IE && INTCONbits.TMR0IF) 
    {
        INTCONbits.TMR0IF = 0;
        TMR0 = 63100; // because after the interrupt this will be 0
        
        //figure out remainder value
        remainder = remainder + remainder_temp;
        
        
        if(remainder >= delay_in) 
        {
            //increase duty cycle by one since (remainder / delay_in) = 1
            duty_cycle = duty_cycle + 1; 
            //recalculate remainder value since (remainder / delay_in) was performed
            remainder = remainder - delay_in;
        }
        
        //figure out duty cycle value
        duty_cycle = duty_cycle + interval_size;

        
        //apply duty cycle value to CCP2
        CCPR2L = ( (duty_cycle & 0x03FC) >> 2 ); //0011 1111 1100 upper 8bit
        CCP2CONbits.DC2B = (duty_cycle & 0x03);  //0000 0000 00011 lower 2bit
        
        
        //check duty cycle for restart
        if(duty_cycle >= max_speed) 
        {
            duty_cycle = 0;
            remainder = 0;
        }
    }

}