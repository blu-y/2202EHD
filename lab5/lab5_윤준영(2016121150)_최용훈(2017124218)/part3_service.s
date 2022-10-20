/*
    part3_service.s
 */
/* RESET SECTION */
    .section	.reset, "ax"
    movia	    r2,     _start
    jmp	        r2                              # branch to main program

/* EXCEPTIONS SECTION */
    .section    .exceptions,    "ax"
    .global     EXCEPTION_HANDLER
EXCEPTION_HANDLER:
    subi        sp,     sp,     16              # make 4 room(et, ea, ra, r22) on the stack
    stw         et,		0(sp)                   # M[sp] <- et
    rdctl       et,	    ctl4                    # et <- ipending
    beq         et,		r0,		SKIP_EA_DEC     # if sw interrupt > br SKIP_EA_DEC
    subi        ea,		ea,		4               # -1 instruction for hw interrupts(ei),
                                                # interrupted instruction will be re-run
                                                # sw interrupt는 감소시키면 계속 trap, so skip
SKIP_EA_DEC:
    stw         ea,		4(sp)                   # save ea, ra, r22, (et is already saved)
    stw		    ra,		8(sp)                   # needed if call inst is used
    stw		    r22,    12(sp)
    rdctl		et,	    ctl4
    bne		    et,		r0,		CHECK_TIMER     # interrupt is an external interrupt

NOT_EI: 
    br		END_ISR                             # must be unimplemented instruction or TRAP
                                                # instruction; ignored in this code
CHECK_TIMER:                                    # check if timer interrupt by IRQ#0
    andi		r22,    et,		0b1
    beq		    r22,    r0,		CHECK_KEY       # if 0, check other IRQ#
    call		TIMER_ISR
    
CHECK_KEY:                                      # check if key interrupt by IRQ#1
    andi		r22,    et,		0b10
    beq		    r22,    r0,		END_ISR         # if 0, end ISR
    call		KEY_ISR
    
END_ISR:
    ldw		    et,		0(sp)                   # restore et, ea, ra, r22
    ldw		    ea,		4(sp)
    ldw		    ra,		8(sp)                   # needed if call inst is used
    ldw		    r22,    12(sp)
    addi		sp,		sp,		16
    eret
    .end