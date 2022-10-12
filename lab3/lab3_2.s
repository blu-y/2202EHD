/*      LAB 3, PART 2
        Write recursive subroutine FINDSUM 
        to compute Sigma i (i=1 to N)
        if N = 9 >> 45
 */
            .text
            .global _start
_start: /* get N and pass to subroutine */
            ldw	    r4,     N(r0)               # r4 <- M[N]
            movia   sp,     0x20000             # Initialize sp
            call    FINDSUM                     # FINDSUM(N)
END:        br      END                         # wait here

FINDSUM:    bne     r4,     r0,     REC         # if r4(N) != 0, branch REC
            mov     r2,     r0                  # r2(sum) = 0
            ret                                 # return
REC:        /* backup r4, ra to sp-8, sp-4 */
            subi    sp,	    sp,	    8           # sp = sp - 8
            stw	    r4,	    0(sp)               # r4 -> M[sp]
            stw	    ra,	    4(sp)               # ra -> M[sp+4]
            
            subi    r4,	    r4,	    1           # r4(N) = r4 - 1
            call    FINDSUM                     # FINDSUM(N-1)
            /* restore r4, ra */
            ldw	    r4,     0(sp)               # r4 <- M[sp]
            ldw	    ra,     4(sp)               # ra <- M[sp+4]
            addi    sp,	    sp,	    8           # sp = sp + 8
            
            add	    r2,	    r4,	    r2          # r2(sum) = r4(N) + r2(sum)
            ret
/* stack overflow occurs when 
 * stack memory is not big enough
 * to do recursive calls 
 */            

N:  	    .word   100
            .end

