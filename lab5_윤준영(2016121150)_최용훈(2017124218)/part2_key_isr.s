/*
    part2_key_isr.s
 */
    .include    "address_map_nios2.s"
    .global     KEY_ISR

KEY_ISR:
    subi		sp,		sp,		8   # backup r22, r23 on stack
    stw		    r22,	0(sp)
    stw		    r23,	4(sp)
    
    movia		r22,	KEY_BASE    # disable IRQ for KEY
    ldwio		r23,	0xC(r22)    # r23 <- edgecapture
    stwio		r23,	0xC(r22)    # r23 -> edgecapture (turn off IRQ)

    movia		r22,	RUN         # toggle RUN
    ldw         r23,    (r22)
    addi		r23,	r23,	1   # and(0b0, 0b0+1) = 0b1
    andi		r23,	r23,	1   # and(0b1, 0b1+1) = 0b0
    stw		    r23,	(r22)
    
    ldw		    r22,	0(sp)       # restore r22, r23 on stack
    ldw		    r23,	4(sp)
    addi		sp,		sp,		8
    ret
    
    .end