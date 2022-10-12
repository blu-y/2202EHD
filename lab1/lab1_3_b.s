/*      PART III : Find largest number in a list
 *      use subroutine LARGE
 */
        .text
        .global     _start
_start:
        movia		r8,	RESULT          #r8 points to the result location
        ldw		r4,	4(r8)           #r4 holds number of elements in the list
        addi		r5,	r8,	8       #r5 points to the start of the list
        call		LARGE
        stw		r2,	0(r8)            #r2 holds the subroutine return value

STOP:   br		STOP

/*      Implement the subroutine LARGE 
        PARAMETER:      number of entries(r4) 
                        address of the start of the list(r5)
        RETURN:         value of the largest number(r2)
*/
LARGE:
        ldw		r6,	(r5)            #r6: current element
        addi		r5,	r5,	4       #r5: addr of next element
        ble             r4,     r0,     EXIT    #if counter<=0, go to EXIT
        subi		r4,	r4,	1       #r4: counter
        bge		r2,	r6,	LARGE   #go to LARGE if r2>=r6
        mov		r2,	r6              #copy r6 to r2 if r2<r6
        br		LARGE
EXIT:   ret

RESULT: .skip           4                       #space for the largest number found
N:      .word	        7                       #number of the entries in the list
NUMBERS:        .word		4, 5, 3, 6      #the data...
                .word           1, 8, 2         #...

        .end
