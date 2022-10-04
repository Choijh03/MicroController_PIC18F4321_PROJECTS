//ECE3301L.02
 //LAB3
 //Jihun Choi
 //This lab is about using a subroutine to do sum of X's and divide by 8
 //using rotate right with carry
	#include <xc.inc>
	goto start	
	
	; Lab 3 Prelab
	psect code
N   equ	0x00	; Number of X's
Z   equ 0x60	; Final answer is stored here
X   equ 0x50	; Starting point of Xi array
Xt  equ 0x20    ; temp location for X 
start:	; load location 0x50 to 0x57 with arbitrary numbers for testing
	; you should also use "Memory Fill" to test it with zeros, ones, or even random numbers
	lfsr	0, X
	movlw	1
	movwf	POSTINC0, a
	movlw	2
	movwf	POSTINC0, a
	movlw	3
	movwf	POSTINC0, a
	movlw	4
	movwf	POSTINC0, a
	movlw	5
	movwf	POSTINC0, a
	movlw	6
	movwf	POSTINC0, a
	movlw	7
	movwf	POSTINC0, a
	movlw	8
	movwf	POSTINC0, a
	
    	; TODO: Your main program code starts here
	; 	- Use FSR1 to point to Xi
	;	- Call sum8 to generate the sum
	;	- Divide the resulting sum by 8 using rotate right with carry (you should reset the carry after each rotation)
	
	movlw 8 // N
	movwf N, a // moving 8 to N
	
	clrf Z,a //clearing Z
	bcf STATUS, 0, a// clearing Carry bit just in case
	
	lfsr 1, X //pointing to start point of X array

loop:  	movff POSTINC1, Xt
        call  sum8, 1
	decfsz N, f, a //if result is zero, skip next line
	bra loop
	
	//sum division by 8  using rotate right with carry
	//reset carry after each rotation
	rrcf Z, f, a
	bcf STATUS, 0, a
	rrcf Z, f, a
	bcf STATUS, 0, a
	rrcf Z, f, a
	bcf STATUS, 0, a
	
	
	
stop: bra stop

sum8:	; TODO: Your subroutine program codes goes here
	movwf Xt, w, a
	addwfc Z, f, a
	clrf Xt, a
	return 1

	
end
