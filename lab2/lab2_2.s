/*      LAB 2, PART 2
        Count the longest string of 1's in words of data
        if 0x103fe00f, 0x3fabedef >> 9
 */
        .text
        .global         _start
_start: mov             r9,     r0                      # r9 = address index of NUMs
        mov             r10,    r0                      # r10 will hold the longest 1's in words(MAX)

LOOP:   /* call ONES iteratively until the end of the words */
        ldw             r4,     TEST_NUM(r9)            # Load the data into r4
        blt             r2,     r10,     PASS           # if r10 is already max, pass next line
        mov             r10,    r2                      # else copy ONES max to MAX
PASS:   mov		r2,	r0                      # r2 will hold the result of ONES (ONES max)
        beq		r4,	r0,	END             # if data is 0 end
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

END:    br              END                             # Wait here

TEST_NUM:       .word	0x3fabedef, 0x103fe00f, 0x0fab34e1, 0xfff21cab, 0x12345678, 0
                                                        # The number to be tested / 9, 7, 5, 12, 4
                            
                             
                             
                        
