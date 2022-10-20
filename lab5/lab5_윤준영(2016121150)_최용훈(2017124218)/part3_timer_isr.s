/*
    part3_timer_isr.s
 */

    .include    "address_map_nios2.s"
    .global     TIMER_ISR

TIMER_ISR:
    subi		sp,		sp,		8   # backup r22, r23 on stack
    stw		    r22,	0(sp)
    stw		    r23,	4(sp)

    movia		r22,    TIMER_BASE
    stwio		r0,		0(r22)      # make TIMER TO = 0
    
    movia		r22,	RUN
    ldw         r23,    (r22)       # load RUN
    beq		    r23,	r0,	    RET # do nothing if RUN = 0
    
    movia		r22,	COUNT
    ldw		    r23,	(r22)       # load COUNT
    addi		r23,	r23,	1   # increase COUNT by 1
    stw		    r23,	(r22)       # store COUNT

RET:
    ldw		    r22,	0(sp)       # restore r22, r23 on stack
    ldw		    r23,	4(sp)
    addi		sp,		sp,		8
    ret
    
    .end