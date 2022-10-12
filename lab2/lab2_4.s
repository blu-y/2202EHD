/*      LAB 2, PART 4
        Count the longest string of 1's in words of data and display
        if 0x103fe00f, 0x3fabedef >> 9 >> 7-seg disp 
 */
        .text
        .include        "address_map_nios2.s"
        .global         _start
_start: mov             r9,     r0                      # r9 = address index(0,4,8,...) of NUMs
        mov             r10,    r0                      # r10 will hold the longest 1's in words(MAX)
        mov             r2,     r0                      # to be initialized (if r2 contains garbage value, discuss)

LOOP:   /* call ONES iteratively until the end of the words */
        ldw             r4,     TEST_NUM(r9)            # Load the data into r4
        blt             r2,     r10,    PASS           # if r10 is already max, pass next line
        mov             r10,    r2                      # else copy ONES max to MAX
PASS:   mov		r2,	r0                      # r2 will hold the result of ONES (ONES max)
        beq		r4,	r0,	DISP             # if data is 0, then display MAX(r10)
        call            ONES
        addi		r9,	r9,	4
        br              LOOP
          
ONES:   /* finds the longest string of 1's in one word
         * input r4 = a word
         * return r2 = longest string of 1's
         */     
        beq		r4,	r0,	ONES_END        # Loop until r9 contains no more 1s
        srli		r5,	r4,	0x01            # Count the 1s by shifting the number and
        and		r4,	r4,	r5              # ANDing it with the shifted result
        addi		r2,	r2,	0x01            # Increment the counter
        br		ONES 
ONES_END:       ret
/* used register r2(ONES max, temp), r4(word, temp), r5(shifted, temp), r9(address index), r10(MAX) */
# now r10 = 20(32)
DISP:   #movia		r8,	0xFF200020              # base address of HEX
        movia		r8,	HEX3_HEX0_BASE          # base address of HEX
        mov		r4,	r10                     # display r10 on HEX1-0
        call		DIVIDE                          # ones digit will be in r2; tens digit in r3
        mov		r4,	r2                      # pass ones digit to SEG7_CODE
        call		SEG7_CODE
        mov		r14,	r2                      # save bit code
        mov		r4,	r3                      # retrieve tens digit, pass to SEG7_CODE
        call            SEG7_CODE
        slli		r2,	r2,	8               # shift 8 digits to be the tens(8digits)ones(8digits)
        or		r14,	r14,	r2              # bit code for tens | bit code for ones
        stwio		r14,	(r8)
        br              END

DIVIDE: mov	        r2,     r4	                #r2 = remainder
	movi	        r5,     10	                #divisor
	movi	        r3,     0	                #r3 = quotient
CONT:	blt	        r2,     r5,     DIV_END
	sub 	        r2,     r2,     r5
	addi	        r3,     r3,     1
	br 	        CONT
DIV_END:        ret

SEG7_CODE:      /* 0~9 to HEX display
                 * input r4 = decimal value of the digit to be displayed
                 * return r2 = bit pattern to be written to the HEX display
                 */
                movia	r15,	BIT_CODES               # starting address of bit codes
                add	r15,	r15,	r4              # index into the bit codes
                ldb	r2,	(r15)                   # read the bit code needed for our digit
                ret

END:    br              END                             # Wait here

BIT_CODES:      .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
                .byte   0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
                .skip   2               # pad with 2 bytes to maintain word alignment

TEST_NUM:       .word	0x3fabedef, 0x103fe00f, 0x0fab34e1, 0xfff21cab, 0x12345678, 0
                                                        # The number to be tested / 9, 7, 5, 12, 4

