/*
    main.s
 */

    .include    "address_map_nios2.s"
    .text
    .global     _start

_start:
    movia		sp,		SDRAM_END   # stack starts from the largest SDRAM address
    /* initialize LEDR value */
    movia		r22,	LEDR_BASE
    stwio		r0,		0(r22)
    
    /* write to the push button port interrupt mask register */
    movia		r22,	KEY_BASE    # pushbutton key base address
    movi		r23,	1           # set interrupt mask bits -> KEY0 interrupt enable
    stwio		r23,	8(r22)      # interrupt mask register is (base+8)
    
    /* enable Nios II processor interrupts */
    movi		r23,	2           # set interrupt mask bit for level 1
    wrctl		ienable,r23
    movi		r23,	1
    wrctl		status,	r23         # turn on Nios II interrupt processing
    
IDLE:
    br		    IDLE
    .end