/*
    part3_key_isr.s
 */
    .include    "address_map_nios2.s"
    .global     KEY_ISR

KEY_ISR:
    subi		sp,		sp,		44   # backup r22, r23, r2, r3, r4, r5, r10, r11, r12, r13, ra on stack
    stw		    r22,	0(sp)
    stw		    r23,	4(sp)
    stw		    r2,	    8(sp)
    stw		    r3,	    12(sp)
    stw		    r4,	    16(sp)
    stw		    r5,	    20(sp)
    stw		    r10,	24(sp)
    stw		    r11,	28(sp)
    stw		    r12,	32(sp)
    stw		    r13,	36(sp)
    stw		    ra,	    40(sp)
    
    movia		r22,	KEY_BASE    # disable IRQ for KEY
    ldwio		r23,	0xC(r22)    # r23 <- edgecapture
    stwio		r23,	0xC(r22)    # r23 -> edgecapture (turn off IRQ)

    movia		r22,	RUN         # toggle RUN
    ldw         r23,    (r22)
    addi		r23,	r23,	1   # and(0b0, 0b0+1) = 0b1
    andi		r23,	r23,	1   # and(0b1, 0b1+1) = 0b0
    stw		    r23,	(r22)
    bne         r23,    r0,     RET # if RUN != 0, SKIP SEG(timer running)

    movia		r22,	COUNT       # load COUNT
    ldw		    r23,    (r22)
    
    mov         r4,     r23
    movi        r5,     1000
    call        DIVIDE
    mov         r10,    r3                          # r10 : THOUSANDS
    mov         r4,     r2
    movi        r5,     100
    call        DIVIDE
    mov         r11,    r3                          # r11 : HUNDREDS
    mov         r4,     r2
    movi        r5,     10
    call        DIVIDE
    mov         r12,    r3                          # r12 : TENS
    mov         r13,    r2                          # r13 : ONES
    br          DISP
DIVIDE:/* input : a(r4) / b(r5)
        * output : q(r3) * b + r(r2)
        */
    mov	        r2,     r4	                        # r2 = remainder
    movi        r3,     0	                        # r3 = quotient
    br          COUNT
COUNT:
    bltu	    r2,     r5,     DIV_END
    sub         r2,     r2,     r5
    addi        r3,     r3,     1
    br 	        COUNT
DIV_END:
    ret

DISP:   /* input : r10(thousands), r11(hundreds), r12(tens), r13(ones) */
SHOW:
    mov		    r4,		r0
    mov		    r3,		r10                         # THOUSANDS
    call        SEG                                 
    slli		r5,		r5,		24
    or		    r4,		r4,		r5
    mov         r3,     r11                         # HUNDREDS
    call        SEG
    slli		r5,		r5,		16
    or		    r4,		r4,		r5
    mov         r3,     r12                         # TENS
    call        SEG             
    slli		r5,		r5,		8
    or		    r4,		r4,		r5
    mov         r3,     r13                         # ONES
    call        SEG
    or		    r4,		r4,		r5
    movia		r22,	HEX3_HEX0_BASE
    stwio	    r4,	    (r22)                        # store at HEX3-0
    br          RET
SEG:/* 0~9 to HEX display
     * input r3 = decimal value
     * output r5 = bit code
     */
    movia	    r23,	BIT_CODES               # starting address of bit codes
    add	        r23,	r23,	r3              # index into the bit codes
    ldb	        r5,	    (r23)                   # read the bit code needed for our digit
    ret
RET:
    ldw		    r22,	0(sp)
    ldw		    r23,	4(sp)
    ldw		    r2,	    8(sp)
    ldw		    r3,	    12(sp)
    ldw		    r4,	    16(sp)
    ldw		    r5,	    20(sp)
    ldw		    r10,	24(sp)
    ldw		    r11,	28(sp)
    ldw		    r12,	32(sp)
    ldw		    r13,	36(sp)
    ldw		    ra,	    40(sp)
    addi		sp,		sp,		44
    ret
    
    .end