/* 
 * Measure the time in milliseconds between the push events
 * 1. Wait for push event
 * 2. Push KEY0 at t1
 * 3. Turn off HEX3-0
 * 4. Push KEY0 again at t2, where t2 is assumed in the range:
    t1 + 5000ms > t2 > t1
    - Display t2-t1 in the decimal format through HEX3-0
    - Go to 1
 */
/*
    part3_main.s
 */

# 1ms = 100000clock = 186A0
    .text
    .include    "address_map_nios2.s"
    .global     _start
_start:
    movia		sp,		SDRAM_END   # stack starts from the largest SDRAM address
    stwio		r0,		0(r8)       # initialize LEDR

    call CONFIG_TIMER
    call CONFIG_KEYS

    /* enable Nios II processor interrupts */
    movi		r23,	0b11        
    wrctl		ienable,r23         # set interrupt mask bit, IRQ#1,2
    movi		r23,	1
    wrctl		status,	r23         # turn on Nios II interrupt processing

LOOP: 
    br		LOOP                    # main idles

    /* Configure the interval timer to create interrupts for every interval */
CONFIG_TIMER:
    movia		r21,	TIMER_BASE  # r21 = TIMER_BASE address
    movui       r14,    0x86A0      # r14 = lo(100K) = 0xD180
    sthio		r14,	8(r21)      # M[0xFF202008](periodl) <- store 0x86A0
    movui       r14,    0x1         # r14 = hi(100K) = 0x8F0
    sthio		r14,	12(r21)     # M[0xFF20200C](periodh) <- store 0x1
    movi		r14,	0b0111      # STOP(0) START(1) CONT(1) ITO(1)
    sthio		r14,	4(r21)      # M[0xFF202004](control) <- store 0b0111
    ret
    /* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:
    movia		r22,	KEY_BASE    # KEY base address
    movi		r23,	0b1         # set interrupt mask bits -> KEY0 interrupt enable
    stwio		r23,	8(r22)      # interrupt mask register is (base+8)
    ret

    /* Global variables */
    .global     COUNT
COUNT:  .word		0x0 # used by timer
    .global     RUN
RUN:    .word       0x1 # initial value to increment COUNT
    .global     BIT_CODES
BIT_CODES:  .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111
            .byte   0b01100110, 0b01101101, 0b01111101, 0b00000111
            .byte   0b01111111, 0b01100111, 0b00000000      # 0 1 2 3 4 5 6 7 8 9 NULL
            .skip   1                                       # pad with 1 bytes to maintain word alignment
    .end
