config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>
goto main	


psect intCodeHi, class = CODE, reloc = 2
    bcf	    INTCON, 1, a
    call subroutine
    retfie  0
	
psect code

main: 

    ; set the I/O port directions
    setf   ADCON1, a   ; AN inputs are now digital inputs
    bcf	    TRISD, 1, a ; RD1 is output (connected to LED)
    bsf	    TRISB, 1, a ; RB1 is input (connected to switch)

     ; Setup interrupts
    movlw   10010000B	
    movwf   INTCON, f, a    ; Enable global interrupt, enable INT0, reset INT0 Flag
    bcf	    INTCON2, 6, a   ; Interrupt on falling edge (default)
    ;bsf	    INTCON2, 6, a   ; Interrupt on rising edge 	
	 
infiniteloop:
 
subroutine:
    btfsc PORTB, 1,a ;if input was 0, skip next line
    bsf PORTD, 1, a ;turn on the led
    btfss PORTB, 1, a ;if input was 1, skip next line
    bcf PORTD, 1, a ;turn off the led
    return
