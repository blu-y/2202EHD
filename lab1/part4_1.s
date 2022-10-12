	.text	
	.global	_start
_start:
	movia 	r4, N
	addi	r8, r4, 4	#r8 points to the decimal digits storage location
	ldw	r4, (r4)

	call	DIVIDE 	#parameter for DIVIDE in in r4
	stb	r3, 1(r8)
	stb	r2, (r8)

END : 	br	END

DIVIDE: 	mov	r2, r4	#r2 = remainder
	movia 	r6, DIV
	ldw	r6, (r6)
	mov	r5, r6	#divisor
	movi	r3, 0	#r3 quotient


CONT:	blt	r2, r5, DIV_END
	sub 	r2, r2, r5
	addi	r3, r3, 1
	br 	CONT

DIV_END:		ret

DIV:	.word	20
N:	.word 	76
Digits:	.space	4
	
	.end 	