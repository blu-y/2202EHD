/* 
 * Write an assembly language program based on interrupt service routines.
 * Your program runs as follows:
 * For every 1.5s, increase COUNT by 1
    (Maybe handled by the ISR for the interval timer)
 * COUNT is displayed through LEDR
 * On pressing and KEYs, toggle RUN, where RUN indicated 
    whether the increment actions is performed or not
    (Maybe handled by the ISR for the parallel port)
 */
/*
    part2_main.s
 */

# 1.5s = 8F0D180
    .text
    .include    "address_map_nios2.s"
    .global     _start
_start:
    movia		sp,		SDRAM_END   # stack starts from the largest SDRAM address
    movia		r8,	    LEDR_BASE   # LEDR base address
    stwio		r0,		0(r8)       # initialize LEDR

    call CONFIG_TIMER
    call CONFIG_KEYS

    /* enable Nios II processor interrupts */
    movi		r23,	0b11        
    wrctl		ienable,r23         # set interrupt mask bit, IRQ#1,2
    movi		r23,	1
    wrctl		status,	r23         # turn on Nios II interrupt processing

LOOP: 
    ldw		r9,		COUNT(r0)       # global variable
    stw		r9,		(r8)
    br		LOOP

    /* Configure the interval timer to create interrupts for every interval */
CONFIG_TIMER:
    movia		r21,	TIMER_BASE  # r21 = TIMER_BASE address
    movui       r14,    0xD180      # r14 = lo(150M) = 0xD180
    sthio		r14,	8(r21)      # M[0xFF202008](periodl) <- store 0xD180
    movui       r14,    0x8F0       # r14 = hi(150M) = 0x8F0
    sthio		r14,	12(r21)     # M[0xFF20200C](periodh) <- store 0x8F0
    movi		r14,	0b0111      # STOP(0) START(1) CONT(1) ITO(1)
    sthio		r14,	4(r21)      # M[0xFF202004](control) <- store 0b0111
    ret
    /* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:
    movia		r22,	KEY_BASE    # KEY base address
    movi		r23,	0b1111      # set interrupt mask bits -> KEY0,1,2,3 interrupt enable
    stwio		r23,	8(r22)      # interrupt mask register is (base+8)
    ret

    /* Global variables */
    .global     COUNT
COUNT:  .word		0x0 # used by timer
    .global     RUN
RUN:    .word       0x1 # initial value to increment COUNT

    .end
