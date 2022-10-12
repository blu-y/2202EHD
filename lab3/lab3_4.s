/*      LAB 3, PART 4
        Write recursive subroutine PRIME 
        to compute the number of the prime numbers
        that are less than or equal to positive N
        if n = 2 >> 1 (2)
        if n = 3 >> 2 (3, 2)
        if n = 5 >> 3 (5, 3, 2)
 */
            .text
            .global _start
_start: /* get N and pass to subroutine */
            ldw	    r4,     N(r0)               # r4 <- M[N]
            movia   sp,     0x20000             # Initialize sp
            call    PRIME                       # PRIME(N)
END:        br      END                         # wait here
/* PRIME(N) = ISPRIME(N) + PRIME(N-1)*/
PRIME:      movi    r7,     2
            bne     r4,     r7,     REC         # if r4(N) != 2, branch REC
            mov     r2,     r0                  # r2(sum) = 0
            ret                                 # return
REC:        /* backup r4, ra to sp-8, sp-4 */
            subi    sp,	    sp,	    8           # sp = sp - 8
            stw	    r4,	    0(sp)               # r4 -> M[sp]
            stw	    ra,	    4(sp)               # ra -> M[sp+4]
            subi    r4,	    r4,	    1           # r4(N) = r4 - 1
            call    PRIME                       # PRIME(N-1)
            /* restore r4, ra */
            ldw	    r4,     0(sp)               # r4 <- M[sp]
            call    ISPRIME                     # ISPRIME(N-1)
            ldw	    ra,     4(sp)               # ra <- M[sp+4]
            addi    sp,	    sp,	    8           # sp = sp + 8
SUM:        add	    r2,	    r2,	    r3          # P(N) = ISPRIME(N) + P(N-1)
            ret
/* ISPRIME(N) = 1 
                if N//(N-1)!=0 and N//(N-2)!=0 and ... and N//2!=0
   input = r4, return = r3
 */
ISPRIME:    mov     r3,     r0
            subi    r5,     r6,     1
LOOP1:      subi    r5,     r5,     1
            mov     r6,     r4
            blt     r5,     r7,     RET1
LOOP2:      sub	    r6,	    r6,	    r5
            blt	    r6,	    r0,	    LOOP1
            beq	    r6,	    r0,	    RET
            br      LOOP2
RET1:       movi    r3,     1
RET:        ret
/* stack overflow occurs when 
 * stack memory is not big enough
 * to do recursive calls 
 */            

N:  	    .word   7
            .end


