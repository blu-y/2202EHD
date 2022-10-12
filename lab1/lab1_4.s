/*      PART IV
        convert a binary number to two decimal digits
        if N = 76(0x4c) >> Digits = 00000706
        extend the code to converts binary to three decimal digits
        (modify DIVIDE subroutine)
        if N = 987(0x3DB) >> Digits = 090807
 */
        .text
        .global     _start
_start:
        movia		r4,	N
        addi		r8,	r4,	4       #r8 points to the decimal digits storage location
        ldw		r4,	(r4)            #r4 holds N
        
        call            DIVIDE                  #parameter for DIVIDE is in r4
        /* Tens digit is now in r3, ones digit is in r2 */
        stb		r3,	1(r8)
        stb		r2,	(r8) 
        ldw             r4,     1(r8)
        call            DIVIDE                  #parameter for DIVIDE is in r4
        /* Tens digit is now in r3, ones digit is in r2 */
        stb		r2,	2(r8)
        stb		r1,	1(r8) 
END:    br              END

/* Subroutine to perform the integer division r4 / 10.
 * Returns: quotient in r3, and remainder in r2
 */

DIVIDE: mov		r2,	r4              #r2 will be the remainder
        movi		r5,	10              #divisor
        movi		r3,	0               #r3 will be the quotient
CONT:   blt		r2,	r5,	DIV_END
        sub		r2,	r2,	r5      #subtract the divisor, and...
        addi		r3,	r3,	1       #increment the quotient
        br              CONT
DIV_END:        ret                             #quotient is in r3, remainder in r2

N:      .word	        987                      #the decimal number to be converted
Digits: .space          6                       #storage space for the decimal digits

        .end
