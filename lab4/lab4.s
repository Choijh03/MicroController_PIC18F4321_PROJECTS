config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>
	goto start	
	
	; If input in C is high & input in D is low, LED at D will be on
	; If input in C is low & input in D is high, LED at C will be on
	; If input in C is high & input in D is high, LED off
	; If input in C is low & input in D is low, LED both on
	; LED turns on whenever is low
	psect code

start:	
	setf	ADCON1, a   ; Turn off ADC inputs
	movlw   0x02     ; b00000010
	movwf	TRISC, a    ; PORTC, bit1-input, bit0-output
	movwf	TRISD, a    ; PORTD, bit1-input, bit0-output
	
infloopc:
	movf	PORTC, w, a
	andlw	00000010B ; keep the values of bit 1
	bz      ledonc	  ;if results are zero, jump to ledonc
	bsf    PORTC, 0, a ; turn off led (cathode connected to port)
	bra    infloopd
	
infloopd:			  		  
	movf	PORTD, w, a
	andlw	00000010B ; keep the values of bit 1
	bz	ledond	  ;if results are zero, jump to ledond
	bsf    PORTD, 0, a ; turn off led (cathode connected to port)
	bra    infloopc
	
ledonc:   bcf	PORTC, 0, a ; turn on led (cathode connected to port)
	   bra	infloopd
	   
ledond:   bcf	PORTD, 0, a ; turn on led (cathode connected to port)
	   bra	infloopc   
end

