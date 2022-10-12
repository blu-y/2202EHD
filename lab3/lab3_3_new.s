/*      LAB 3, PART 3
        Write recursive subroutine FINDSUM to compute 3^n
        if n = 0 >> 1
        if n = 3 >> 27
 */
            .text
            .global _start
_start: /* get N and pass to subroutine */
            ldw	    r4,     N(r0)               # r4 <- M[N]
            movia   sp,     0x20000             # Initialize sp
            call    POW3                        # POW3(N)
END:        br      END                         # wait here

POW3:       movi    r2,     1                   # r2(pow) = 1
            bne     r4,     r0,     REC         # if r4(N) != 0, branch REC
            ret                                 # return
REC:        /* backup r4, ra to sp-8, sp-4 */
            subi    sp,     sp,	    4           # sp = sp - 8
            stw	    ra,     (sp)               # ra -> M[sp+4]
            
            subi    r4,     r4,     1           # r4(N) = r4 - 1
            call    POW3                        # POW3(N-1)
            /* restore r4, ra */
            ldw	    ra,     (sp)               # ra <- M[sp+4]
            addi    sp,     sp,     4           # sp = sp + 8
            
            muli    r2,	    r2,     3           # r2(pow) = r2(pow) x 3
            ret
/* stack overflow occurs when 
 * stack memory is not big enough
 * to do recursive calls 
 */            

N:  	    .word   20
            .end

