 //ECE3301L.02
 //LAB1
 //Jihun Choi
 //This lab is about adding two 24-bit data items in memory
 //final answer is stored in locations 0x50, 0x51, 0x52
    #include <xc.inc>
    ;psect code

    //labeling
    num_AH equ 0x21 //MSB of A
    num_AM equ 0x22
    num_AL equ 0x23 //LSB of A
    num_BH equ 0x31 //MSB of B
    num_BM equ 0x32 
    num_BL equ 0x33 //LSB of B
    num_tempH equ 0x41 //MSB of temp saver
    num_tempM equ 0x42
    num_tempL equ 0x43 //LSB of temp saver
    num_carryOut equ 0x40 
    num_CH equ 0x50 //MSB of C
    num_CM equ 0x51
    num_CL equ 0x52 //LSB of C
 
 
    //A = 0xF191B5, AH = 0xF1, AM =0x91, AL =0xB5
    MOVLW 0xf1
    MOVWF num_AH, f, a
    MOVLW 0x91
    MOVWF num_AM, f, a
    MOVLW 0xB5
    MOVWF num_AL, f, a
    
    //B = 0x07A204 BH= 0x07, BM = 0xA2, BL = 0x04
    MOVLW 0x07
    MOVWF num_BH, f, a
    MOVLW 0xA2
    MOVWF num_BM, f, a
    MOVLW 0x04
    MOVWF num_BL, f, a
    
    clrf WREG, a //clearing WREG
    
    //sum low byte first
    MOVWF num_AL, w, a
    ADDWF num_BL, w, a
    MOVWF num_tempL, f, a
    
    //sum middle byte 
    MOVWF num_AM, w, a
    addwfc num_BM, w, a //adding carry out bit from low byte sum
    movwf num_tempM, f, a
    
    //sum high byte 
    MOVWF num_AH, w, a
    addwfc num_BH, w, a //adding carry out bit from middle byte sum
    movwf num_tempH, f, a
    
    MOVF num_carryOut, w, a
    addwfc num_carryOut, f, a
    //After checking carry out, moving answer to final location
    //if 0x40 is not 00, that means there was carry out from sum of high byte
    MOVFF num_tempL, num_CL, a
    MOVFF num_tempM, num_CM, a
    MOVFF num_tempH, num_CH, a
    
    //Final answer is MSB(0x50),(0x51), LSB(0x52) 
 
 end
