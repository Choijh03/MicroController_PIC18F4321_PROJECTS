config OSC = INTIO2
config BOR = OFF        ; Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
config STVREN = OFF     ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will not cause Reset)
config WDT = OFF        ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
config MCLRE = ON       ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#include <xc.inc>	
	goto start	
	
	; Convert Hexa into 7 Segment pattern
	; Start by copying the program from subroutine
	
	psect data  
lookup:	DB  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86	
	
	psect code
SSEG	EQU 0x41    ; 7segment pattern table starting address in data memory
I	EQU 0x70    ; used as a counter index
num_temp EQU 0x30	
num_a	EQU 0x50    ;num A
num_b	EQU 0x60    ;num B
start:	
	; Move the 7seg pattern from program memory into data memory
	movlw	low lookup
	movwf	TBLPTRL, f, a
	movlw	high lookup
	movwf	TBLPTRH, f, a
	movlw	low highword lookup
	movwf	TBLPTRU, f, a
	
	lfsr	0, SSEG ; starting address in data memory
	movlw	15
	movwf	I, f, a ; initialize counter with 16

loop:	TBLRD*+    ; read 1B from program memory and advance TBLPTR by 1
	movff	TABLAT, POSTINC0 ;copy TABLAT into INDF0 them move FSR0 pointer forward
	decf	I, f, a;
	bnz	loop
	
	
	; set the I/O port directions
	setf	ADCON1, a   ; turn off the ADC
	clrf	TRISA, a    ; output connected to 7seg
	setf	TRISD, a    ; input  connected to 6 switches
	
	; start the read/display
infiniteloop:
	movf	PORTD, w, a ; read value from switches
	andlw	0x77	    ; keep b01110111 0~2 and 4~6 for input
	movwf   num_temp, a ;saving it to use it again
	
	andlw 0x0F          ;mask it to have only lower 4 bit (0~2)
	movwf num_a, a      ;save it in num_a
	
	movf  num_temp, w, a ;move original input working register
	andlw 0xF0           ;mask it to have only upper 4bit (4~6)
	movwf num_b          ;saving it in num_b
	rrncf num_b, f, a    ;shifiting 4 times to have it in right location
	rrncf num_b, f, a
	rrncf num_b, f, a
	rrncf num_b, f, a
	
	movf num_a, w, a     ;bring num_a value to working register
	addwf num_b, w, a    ;doing a + b  in working register 
	andlw 0x0F           ;we only care for lowest 4 bits only
	call	Hex2sseg, 0
	movwf	PORTA, a
	bra	infiniteloop
    
	; convert a HEX pattern stored in 4 lsb of WREG into 7Seg
Hex2sseg: 
	lfsr	0, SSEG; move fsr0 pointer back to start of table
	movf	PLUSW0, w, a
	return 0; WREG will have the sseg pattern upon return
	
end
