/* 
 * Write an assembly language program based on interrupt service routines.
 * Your program runs as follows:
 * KEY0 > toggle '0' / blank on HEX0
 * KEY1 > toggle '1' / blank on HEX1
 * KEY2 > toggle '2' / blank on HEX2
 * KEY3 > toggle '3' / blank on HEX3
 */

    .text
    .global         _start
_start:
    /* set up the stack */
    
    # code

    /* write to the pushbutton port interrupt mask register */

    # code

    /* enable Nios II processor interrupts */

    # code

IDLE:   
    br		    IDLE                            # main program simply idles
    .end

/* RESET SECTION */
    .section	.reset, "ax"
    movia	    r2,     _start
    jmp	        r2                              # branch to main program

/* EXCEPTIONS SECTION */
    .section    .exceptions,    "ax"
    .global     EXCEPTION_HANDLER
EXCEPTION_HANDLER:
    subi        sp,     sp,     16              # make room on the stack
    stw         et,		0(sp)
    rdctl       et,	    ctl4
    beq         et,		r0,		SKIP_EA_DEC     # interrupt is not external
    subi        ea,		ea,		4               # must decrement ea by one instruction for external interrupts, so that the interrupted instruction will be re-run
SKIP_EA_DEC:
    stw         ea,		4(sp)                   # save all used registers onthe Stack
    stw		    ra,		8(sp)                   # needed if call inst is used
    
    stw		    r22,    12(sp)
    rdctl		et,	    ctl4
    bne		    et,		r0,		CHECK_LEVEL_1   # interrupt is an external interrupt
NOT_EI: 
    br		END_ISR                             # must be unimplemented instruction or TRAP
                                                # instruction; ignored in this code
CHECK_LEVEL_1:                                  # pushbutton port is interrupt level 1
    andi		r22,    et,		0b10
    beq		    r22,    r0,		END_ISR         # other interrupt levels are not handled in this code
    call		KEY_ISR
    
END_ISR:
    ldw		    et,		0(sp)                   # restore all used register to previous values
    ldw		    ea,		4(sp)
    ldw		    ra,		8(sp)                   # needed if call inst is used
    ldw		    r22,    12(sp)
    addi		sp,		sp,		16
    eret
    
    .end
