                    .text
                    .include    "address_map_nios2.s"
                    .global     _start
_start:             movia       r7,     KEY_BASE                    # base address of KEY (0xFF200050)
                    movia		r8,	    HEX3_HEX0_BASE              # base address of HEX (0xFF200020)
                    movia       sp,     0x20000                     # Initialize stack pointer
                    br          RESET_CALL                          # branch RESET_CALL to initialize r3(num) = 0
MAIN:               /* KEY0 pressed, r6 = M[r7] = 0b0001
                     * KEY1 pressed, r6 = M[r7] = 0b0010
                     * KEY2 pressed, r6 = M[r7] = 0b0100
                     * KEY3 pressed, r6 = M[r7] = 0b1000
                     */
KEY_PRESS_WAIT:     ldwio		r6,		0(r7)                       # load KEY
                    beq		    r6,		r0,		KEY_PRESS_WAIT      # wait until KEY is pressed / polled IO
KEY_RELEASE_WAIT:   ldwio		r5,		0(r7)                       # load KEY
                    bne		    r5,		r0,		KEY_RELEASE_WAIT    # wait until KEY is released / polled IO
                    /* shift r6 by 1 digit
                     * r4 = and operation r6 with 1
                     * r4 == 1 (r4 != 0) if corresponding KEY is pressed 
                     * branch corresponding Subroutine
                     */
                    andi		r4,		r6,		0x1                 # r4 = and(r6, 0x1)
                    bne		    r4,		r0,		RESET_CALL          # branch RESET_CALL if r4!=0
                    srli		r6,		r6,		0x1                 # shift_right(r6,1)
                    andi		r4,		r6,		0x1                 # r4 = and(r6, 0x1)
                    bne		    r4,		r0,		INCREMENT_CALL      # branch INCREMENT_CALL if r4!=0
                    srli		r6,		r6,		0x1                 # shift_right(r6,1)
                    andi		r4,		r6,		0x1                 # r4 = and(r6, 0x1)
                    bne		    r4,		r0,		DECREMENT_CALL      # branch DECREMENT_CALL if r4!=0
                    srli		r6,		r6,		0x1                 # shift_right(r6,1)
                    andi		r4,		r6,		0x1                 # r4 = and(r6, 0x1)
                    bne         r4,     r0,     OFF_CALL            # branch OFF_CALL if r4!=0
INCREMENT_CALL:     call		INCREMENT
                    br		    MAIN
DECREMENT_CALL:     call        DECREMENT
                    br          MAIN
RESET_CALL:         call        RESET
                    br          MAIN
OFF_CALL:           call        OFF
                    br          MAIN
INCREMENT:          movi		r5,		9                          
                    beq         r3,     r5,     RET                 # if num = 9, then return
                    addi		r3,		r3,		1                   # num = num + 1
                    subi        sp,     sp,	    4                   # sp = sp - 4
                    stw	        ra,     0(sp)                       # backup return address
                    call		SEG                                 # call 7seg-display
                    ldw	        ra,     0(sp)                       # restore return address
                    addi        sp,	    sp,	    4                   # sp = sp + 4
RET:                ret
DECREMENT:          beq         r3,     r0,     RET                 # if num = 0, then return
                    subi		r3,		r3,		1                   # num = num - 1         
                    subi        sp,     sp,	    4                   # sp = sp - 4
                    stw	        ra,     0(sp)                       # backup return address
                    call		SEG                                 # call 7seg-display
                    ldw	        ra,     0(sp)                       # restore return address
                    addi        sp,	    sp,	    4                   # sp = sp + 4
                    ret
RESET:              mov         r3,     r0                          # num = 0
                    subi        sp,     sp,	    4                   # sp = sp - 4
                    stw	        ra,     0(sp)                       # backup return address
                    call		SEG                                 # call 7seg-display
                    ldw	        ra,     0(sp)                       # restore return address
                    addi        sp,	    sp,	    4                   # sp = sp + 4
                    ret
OFF:                movi		r3,		10                          # num = 10 (temp)
                    subi        sp,     sp,	    4                   # sp = sp - 4
                    stw	        ra,     0(sp)                       # backup return address
                    call		SEG                                 # call 7seg-display
                    ldw	        ra,     0(sp)                       # restore return address
                    addi        sp,	    sp,	    4                   # sp = sp + 4
                    mov         r3,     r0                          # num = 0
                    ret

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
                    .byte   0b01111111, 0b01100111, 0b00000000      # 0 1 2 3 4 5 6 7 8 9 NULL
                    .skip   1                                       # pad with 1 bytes to maintain word alignment

