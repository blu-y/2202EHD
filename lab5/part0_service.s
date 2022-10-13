/*
    service.s
 */

    .section	.reset, "ax"                    # Allocatable and eXecutable
    movia		r2,		_start
    jmp		    r2
    .section	.exceptions,    "ax"
    .global     ISR

ISR:
    subi		sp,		sp,		16              # et, ea, ra, r22 will be stored into memory
    stw		    et,		0(sp)
    rdctl		et,		ctl4                    # et is 0 only if no external (hardware) interrupts occurred; i.e. trap
    beq		    et,		r0,		SKIP_EA_DEC
    subi		ea,		ea,		4               # ea is decreased only for trap

SKIP_EA_DEC:
    stw		    ea,		4(sp)
    stw		    ra,		8(sp)
    stw		    r22,	12(sp)
    rdctl		et,     ctl4
    bne		    et,		r0,		ON_EXT_INT

ON_TRAP:
    br		    END_ISR
    
ON_EXT_INT:
    andi		r22,	et,		0b10            # check KEY IRQ
    beq		    r22,	r0,		END_ISR
    call		ON_KEY_PRESS
    
END_ISR:
    ldw		    et,		0(sp)                   # et, ea, ra, r22 are restored from memory
    ldw		    ea,		4(sp)
    ldw		    ra,		8(sp)
    ldw		    r22,	12(sp)
    addi		sp,		sp,		16
    eret
    .end
