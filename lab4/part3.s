                    .text
                    .include    "address_map_nios2.s"
                    .global     _start
_start:             movia       r7,     KEY_BASE                    # base address of KEY (0xFF200050)
                    movia		r8,	    HEX3_HEX0_BASE              # base address of HEX (0xFF200020)
                    movia		r9,	    SW_BASE                     # base address of SW (0xFF200040)
# _start:             movia       r7,     0xFF200050                  # base address of KEY (0xFF200050)
#                     movia		r8,	    0xFF200020                  # base address of HEX (0xFF200020)
#                     movia		r9,		0xFF200040                  # base address of SW (0xFF200040)
                    
                    movia       sp,     0x20000                     # Initialize stack pointer
MAIN:               /* KEY0 pressed, r6 = M[r7] = 0b01
                     * KEY1 pressed, r6 = M[r7] = 0b10
                     */
                    ldwio		r3,		0(r9)                       # r3 = SWITCH 9~0
                    srli        r2,     r3,     5                   # r2(a) = r3 >> 5 = SWITCH 9~5
                    andi		r3,		r3,		0b11111             # r3(b) = and(r3, 0b11111) = SWITCH 4~0
                    srli        r4,     r2,     4                   # sign extension
                    slli        r4,     r4,     31
                    andi		r2,		r2,		0b1111              # r2(a) = SWITCH 8~5 (number part)
                    or		    r2,		r4,		r2
                    srli        r4,     r3,     4                   # sign extension
                    slli        r4,     r4,     31
                    andi		r3,		r3,		0b1111              # r3(b) = SWITCH 3~0 (number part)
                    or		    r3,		r4,		r3
                    ldwio		r6,		0(r7)                       # load KEY              
                    andi		r4,		r6,		0b11                # r4 = and(r6, 0b11) get last 2 digit
                    beq		    r4,		r0,		PLUS                # if r4==0(r6=0b00) branch PLUS 
                    movi		r5,		1                        
                    beq		    r4,		r5,		MINUS               # if r4==1(r6=0b01) branch MINUS
                    br		    MULTIPLY                            # else branch MULTIPLY
PLUS:               add		    r3,		r2,		r3                  # r3(result) = r2(a) + r3(b)
                    br          DIGIT
MINUS:              sub		    r3,		r2,		r3                  # r3(result) = r2(a) - r3(b)
                    br          DIGIT
MULTIPLY:           mul		    r3,		r2,		r3                  # r3(result) = r2(a) * r3(b)
                    br          DIGIT

DIGIT:              /* input : r3 = result
                     * output : r10(hundreds), r11(tens), r12(ones)
                     */
                    movi		r13,	1
                    bge		    r3,		r0,		SKIP
COMP:               nor		    r3,		r3,		r3
                    andi        r3,     r3,     0b1111
                    addi		r3,		r3,		1
                    movi		r13,	0
SKIP:               mov         r4,     r3
                    movi        r5,     100
                    call        DIVIDE
                    mov         r10,    r3                          # r10 : HUNDREDS
                    mov         r4,     r2
                    movi        r5,     10
                    call        DIVIDE
                    mov         r11,    r3                          # r11 : TENS
                    mov         r12,    r2                          # r12 : ONES
                    br          DISP
DIVIDE:             /* input : a(r4) / b(r5)
                     * output : q(r3) * b + r(r2)
                     */
                    mov	        r2,     r4	                        # r2 = remainder
	                movi        r3,     0	                        # r3 = quotient
                    br          COUNT
COUNT:	            blt	        r2,     r5,     DIV_END
	                sub         r2,     r2,     r5
	                addi        r3,     r3,     1
	                br 	        COUNT
DIV_END:            ret

DISP:               /* input : r10(hundreds), r11(tens), r12(ones), r13(pos) */
                    mov         r4,     r0
                    bne		    r13,	r0,		POSITIVE
                    movi		r3,		10                          # SIGN (-)
                    br		SHOW        
POSITIVE:           movi        r3,     11                          # SIGN (+ = no sign)
SHOW:               call        SEG
                    slli		r5,		r5,		24
                    or		    r4,		r4,		r5
                    mov         r3,     r10                         # HUNDREDS
                    call        SEG             
                    slli		r5,		r5,		16
                    or		    r4,		r4,		r5
                    mov         r3,     r11                         # TENS
                    call        SEG
                    slli		r5,		r5,		8
                    or		    r4,		r4,		r5
                    mov         r3,     r12                         # ONES
                    call        SEG
                    or		    r4,		r4,		r5
                    stwio	    r4,	    (r8)                        # store at HEX0
                    br          MAIN
SEG:                /* 0~9 to HEX display
                     * input r3 = decimal value
                     * output r5 = bit code
                     */
                    movia	    r15,	BIT_CODES               # starting address of bit codes
                    add	        r15,	r15,	r3              # index into the bit codes
                    ldb	        r5,	    (r15)                   # read the bit code needed for our digit
                    ret
BIT_CODES:          .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111
                    .byte   0b01100110, 0b01101101, 0b01111101, 0b00000111
                    .byte   0b01111111, 0b01100111, 0b01000000, 0b00000000      # 0 1 2 3 4 5 6 7 8 9 - NULL