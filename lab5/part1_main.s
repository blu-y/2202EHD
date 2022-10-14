/* 
 * Write an assembly language program based on interrupt service routines.
 * Your program runs as follows:
 * KEY0 > toggle '0' / blank on HEX0
 * KEY1 > toggle '1' / blank on HEX1
 * KEY2 > toggle '2' / blank on HEX2
 * KEY3 > toggle '3' / blank on HEX3
 */

/*
    part1_main.s
 */
    .include    "address_map_nios2.s"
    .text
    .global     _start
_start:
    /* set up the stack */
    movia		sp,		SDRAM_END   # stack starts from the largest SDRAM address
    movia       r21,    HEX3_HEX0_BASE
    stwio       r0,     0(r21)      # initialize HEX
    /* write to the push button port interrupt mask register */
    movia		r22,	KEY_BASE    # pushbutton key base address
    movi		r23,	0b1111      # set interrupt mask bits -> KEY0123 interrupt enable
    stwio		r23,	8(r22)      # interrupt mask register is (base+8)
    /* enable Nios II processor interrupts */
    movi		r23,	2           # set interrupt mask bit for level 1
    wrctl		ienable,r23         # IRQ 1 = pushbutton switch parallel port
    movi		r23,	1           # turn on Nios II interrupt processing
    wrctl		status,	r23         # PIE = 1 (IRQ enable)
IDLE:   
    br		    IDLE                # main program simply idles
    .end