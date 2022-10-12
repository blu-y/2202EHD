/*      LAB 3, PART 1
        Write subroutine FINDSUM using loop
        to compute Sigma i (i=1 to N)
        if N = 9 >> 45
 */
            .text
            .global _start
_start: /* get N and pass to subroutine */
            ldw	    r4,	    N(r0)               # load word N
            call    FINDSUM                     # FINDSUM()
END:        br      END                         # wait here

FINDSUM:    mov     r2,     r0                  # r2(sum) = 0
WHILE:      beq	    r4,     r0,	    RET         # while r4(N) != 0      
            add     r2,     r2,     r4          # r2(sum) = r2(sum) + r4(N)
            subi    r4,	    r4,	    1           # r4(N) = r4(N) - 1
            br      WHILE                       # repeat
RET:        ret                                 # return

N:  	    .word   9
            .end

