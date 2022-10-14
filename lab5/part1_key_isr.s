/*
    part1_key_isr.s
 */

    .include    "address_map_nios2.s"
    .global     KEY_ISR

KEY_ISR:
    subi		sp,		sp,		20  # backup registers
    stw		    r22,	0(sp)
    stw		    r23,	4(sp)
    stw		    r15,	8(sp)
    stw		    r3,	    12(sp)
    stw		    r20,	16(sp)
    
    movia		r22,	KEY_BASE
    ldwio		r23,	0xC(r22)    # r23 <- edgecapture
    stwio		r23,	0xC(r22)    # r23 -> edgecapture (turn off IRQ)
    
    movi		r3,		0    
    andi        r20,    r23,    1   # KEY0 check
    bne		    r20,	r0,		SEG # r20 !=0 if KEY0 pressed
    addi		r3,		r3,		1   # r3 = r3 + 1 (if KEY0 not pressed)
    srli		r23,	r23,	1   # r23>>1
    andi        r20,    r23,    1   # KEY1 check
    bne		    r20,	r0,		SEG # r20 !=0 if KEY1 pressed
    addi		r3,		r3,		1   # r3 = r3 + 1 (if KEY1 not pressed)
    srli		r23,	r23,	1   # r23>>1
    andi        r20,    r23,    1   # KEY2 check
    bne		    r20,	r0,		SEG # r20 !=0 if KEY2 pressed
    addi		r3,		r3,		1   # r3 = r3 + 1 (then KEY3 pressed)
    
SEG:
    movia		r22,	HEX3_HEX0_BASE
    movia		r15,	BIT_CODES   # address of the bit codes
    add		    r15,	r15,	r3  # index into the bit codes
    ldwio		r23,	0(r22)      # read the previous value
    beq			r23,	r0,		OFF # if hex is off, SKIP
    movi		r3,		10          # else(hex is on), make blank
OFF:ldb		    r20,	(r15)
    muli		r3,		r3,		8
    sll		    r20,	r20,	r3  # r20<<(8 x num)
    or		    r20,	r23,	r20 # r20 = or(prev, r20)
    stwio		r20,	(r22)       # store BITCODE
    ldw		    r22,	0(sp)       # restore registers
    ldw		    r23,	4(sp)
    ldw		    r15,	8(sp)
    ldw		    r3,	    12(sp)
    ldw		    r20,    16(sp)
    addi		sp,		sp,		20
    ret
    .end

BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111
            .byte   0b01100110, 0b01101101, 0b01111101, 0b00000111
            .byte   0b01111111, 0b01100111, 0b00000000  # 0 1 2 3 4 5 6 7 8 9 -
            .skip   1                                   # pad with 2 bytes to maintain word alignment

