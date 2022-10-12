                    .text
                    .include    "address_map_nios2.s"
                    .global     _start
_start:             movia       r7,     TIMER_BASE                  # base address of TIMER (0xFF202000)
                    movia		r8,	    HEX3_HEX0_BASE              # base address of HEX (0xFF200020)
                    movia       sp,     0x20000                     # Initialize stack pointer
                    mov         r3,     r0                          # Initialize r3(sec)
                    movui       r14,    0xE100                      # r14 = lo(100M) = 0xE100
                    sthio		r14,	8(r7)                       # M[0xFF202008](periodl) <- store 0xE100
                    movui       r14,    0x5F5                       # r14 = hi(100M) = 0x5F5
                    sthio		r14,	12(r7)                      # M[0xFF20200C](periodh) <- store 0x5F5
                    movi		r14,	0b0110                      # STOP(0) START(1) CONT(1) ITO(0)
                    sthio       r14,    4(r7)                       # M[0xFF202004](control) <- store 0b0110
                    /* Timer starts */
MAIN:               call        SEG                                 # display 7-seg
                    ldwio		r2,		0(r7)                       # load Status register to r2
                    andi		r2,		r2,		1                   # and operation with 1
                    beq		    r2,		r0,		MAIN                # r2 = 0 means TO != 1 then skip / polled IO
                    addi		r3,		r3,		1                   # r3(sec) = r3 + 1
                    stwio		r0,		0(r7)                       # make TO = 0
                    movi		r2,		10
                    bge		    r3,     r2,     WRAP                # if r3(sec)>=10, then branch WRAP
                    br          MAIN                                # loop
WRAP:               mov         r3,     r0                          # make r3(sec) = 0
                    br          MAIN                                # loop
SEG:                /* 0~9 to HEX display
                     * input r3 = decimal value of the digit to be displayed
                     * show bit pattern to be written to the HEX display
                     */
                    movia	        r15,	BIT_CODES               # starting address of bit codes
                    add	            r15,	r15,	r3              # index into the bit codes
                    ldb	            r2,	    (r15)                   # read the bit code needed for our digit
                    stwio		    r2,	    (r8)                    # store at memory mapped IO
                    ret
BIT_CODES:          .byte   0b00111111, 0b00000110, 0b01011011, 0b01001111
                    .byte   0b01100110, 0b01101101, 0b01111101, 0b00000111
                    .byte   0b01111111, 0b01100111                  # 0 1 2 3 4 5 6 7 8 9
                    .skip   2                                       # pad with 2 bytes to maintain word alignment

