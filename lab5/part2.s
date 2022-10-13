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

    .text
    .global     _start
_start:
    /* set up the stack */
    
    # code

    call CONFIG_TIMER
    call CONFIG_KEYS

    /* enable Nios II processor interrupts */

    # code

    movia		r8,		/*inset red lights LEDR base address*/
LOOP:   ldw		r9,		COUNT(r0)   # global variable
        stw		r9,		(r8)
        br		LOOP

    /* Configure the interval timer to create interrupts for every interval */
CONFIG_TIMER:

    # code
    ret

    /* Configure the pushbutton KEYS to generate interrupts */
CONFIG_KEYS:

    # code
    ret

    /* Global variables */
    .global     COUNT
COUNT:  .word		0x0 # used by timer
    .global     RUN
RUN:    .word       0x1 # initial value to increment COUNT

    .end
