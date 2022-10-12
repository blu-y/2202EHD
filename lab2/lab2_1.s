/*      LAB 2, PART 1
        Count the longest string of 1's in a word of data
        if 0x103fe00f >> 9
 */
        .text                                           # this segment is a program
        .global     _start                              # make symbol _start accessible (assembler makes symbols to binary codes)
_start:
        ldw		r9,	TEST_NUM(r0)            # Load the data into r9
        mov		r10,	r0                      # r10 will hold the result(initialize)

LOOP:   beq		r9,	r0,	END             # Loop until r9 contains no more 1s
        srli		r11,	r9,	0x01            # Count the 1s by shifting the number and
        and		r9,	r9,	r11             # ANDing it with the shifted result
        addi		r10,	r10,	0x01            # Increment the counter
        br		LOOP 

END:    br              END                             # Wait here

TEST_NUM:       .word		0x3fabedef              # The number to be tested

/*
r9  < 00111111101010111110110111101111 (0x3fabedef)
r11 < 00011111110101011111011011110111
r9  < 00011111100000011110010011100111
r10 < 1
r11 < 00001111111010101111101101111011
r9  < 00001111100000001110000001100011
r10 < 2
r11 < 00000111110000000111000000110001
r9  < 00000111100000000110000000100001
r10 < 3
r11 < 00000011110000000011000000010000
r9  < 00000011100000000010000000000000
r10 < 4
r11 < 00000001110000000001000000000000
r9  < 00000001100000000000000000000000
r10 < 5
r11 < 00000000110000000000000000000000
r9  < 00000000100000000000000000000000
r10 < 6
r11 < 00000000010000000000000000000000
r9  < 00000000000000000000000000000000
r10 < 7
END
 */