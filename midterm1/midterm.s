config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	goto start	
	
	
	psect data
lookup0:       DB  0xFF, 0xFE  ;off and on 
lookup1:       DB  0x9F, 0xBD, 0xF9, 0xF3, 0xE7, 0xCF
lookup2:	DB  0xCF, 0xE7, 0xF3, 0xF9, 0xBD, 0x9F	
	
	
	psect code
N0     EQU 0x50
N1     EQU 0x51
N2     EQU 0x52
SSEG0	EQU 0x41    ; 7segment pattern table starting address in data memory
SSEG1   EQU 0x11 	
SSEG2   EQU 0x21 
I0	EQU 0x70    ; used as a counter index
I1	EQU 0x71    ; used as a counter index
I2	EQU 0x72    ; used as a counter index

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;moving data for pattern 01;;;;;;;;;;
start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup0
	movwf	TBLPTRL, f, a
	movlw	high lookup0
	movwf	TBLPTRH, f, a
	movlw	low highword lookup0
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG0 ; starting address in data memory
	movlw	2
	movwf	I0, f, a ; initialize counter with 2
loop0:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I0, f, a;
	bnz	loop0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;moving data for pattern 10;;;;;;;;;;
	movlw	low lookup1
	movwf	TBLPTRL, f, a
	movlw	high lookup1
	movwf	TBLPTRH, f, a
	movlw	low highword lookup1
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG1 ; starting address in data memory
	movlw	6
	movwf	I1, f, a ; initialize counter with 6
loop1:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I1, f, a;
	bnz	loop1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;moving data for pattern 11;;;;;;;;;;	

	movlw	low lookup2
	movwf	TBLPTRL, f, a
	movlw	high lookup2
	movwf	TBLPTRH, f, a
	movlw	low highword lookup2
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG2 ; starting address in data memory
	movlw	6
	movwf	I2, f, a ; initialize counter with 6
loop2:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I2, f, a;
	bnz	loop2

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;setting the port;;;;;;;;;;;;;;;;;;;;;;;	
	
	; set the I/O port directions
	setf	ADCON1, a   ; turn off the ADC
	clrf	TRISA, a    ; output connected to 7seg
	setf	TRISD, a    ; input  connected to 4 switches
	
	; start the read/display

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;infinite loop;;;;;;;;;;;;;;;;;;;;;;;	
infiniteloop:
    ;is bit 0 equal 0 or 1?
    movf PORTD, w, a
    andlw 0x01
    sublw 0x01
    bz  bit0x ;bit0 is 1
    bra bit0y ;bit0 is 0
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;	
bit0x: ;bit0 is 1
    movf PORTD, w, a
    andlw 0x02
    sublw 0x02
    bz  bit11 ;bit1 is 1 
    bra bit01 ;bit1 is 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 bit0y: ;bit0 is 0
    movf PORTD, w, a
    andlw 0x02
    sublw 0x02
    bz  bit10 ;bit1 is 1 
    bra bit00 ;bit1 is 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
 bit00:
    movlw 0xFF
    movwf PORTA, a
    bra infiniteloop   
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;   
bit01:
    movlw 2 
    movwf N0, a
    lfsr 1, SSEG0 ;pointing to start of SSEG0
loop01: 
    call delay500ms
    movff POSTINC1, PORTA
    decfsz N0, f, a //if result is 0 skip next line
    bra loop01
    bra infiniteloop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;    
bit10:
    movlw 6 
    movwf N1, a
    lfsr 1, SSEG2 ;pointing to start of SSEG0
loop10: 
    call delay500ms
    movff POSTINC1, PORTA
    decfsz N1, f, a //if result is 0 skip next line
    bra loop10
    bra infiniteloop
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;      
bit11:
    movlw 6 
    movwf N2, a
    lfsr 1, SSEG1 ;pointing to start of SSEG0
loop11: 
    call delay500ms
    movff POSTINC1, PORTA
    decfsz N2, f, a //if result is 0 skip next line
    bra loop11
    bra infiniteloop   
   
    
 
    
    
    
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;delay function;;;;;;;;;;;;;;;;;;;;;;;	
delay2550us:			    ; 2550 us delay
	movlw	255
l1:	decf	WREG, w, a
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	bnz	l1
	return 1

delay500ms:			    
	movlw	100		    
	movwf	0x10, a
l2:	call	delay2550us
	decf	0x10, f, a
	bnz	l2
	return 1
end
