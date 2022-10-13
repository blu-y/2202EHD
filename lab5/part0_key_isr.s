/*
    key_isr.s
 */

    .include    "address_map_nios2.s"
    .global     ON_KEY_PRESS

ON_KEY_PRESS:
    subi		sp,		sp,		8
    stw		    r22,	0(sp)
    stw		    r23,	4(sp)
    
    movia		r22,	KEY_BASE
    ldwio		r23,	0xC(r22)
    stwio		r23,	0xC(r22)
    
    movia		r22,	LEDR_BASE
    ldwio		r23,	0(r22)
    addi		r23,	r23,		1
    stwio		r23,	0(r22)
    
    ldw		    r22,	0(sp)
    ldw		    r23,	4(sp)
    addi		sp,		sp,		8
    ret
    
    .end