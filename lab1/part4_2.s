	.text	
	.global	_start
_start:
	movia 	r4, N
	addi	r8, r4, 4	#r8 points to the decimal digits storage location
	ldw	r4, (r4)

	call	DIVIDE 	#parameter for DIVIDE in in r4
	call 	DIV_SEC
	stb	r7, 2(r8)	
	stb	r6, 1(r8) 
	stb	r2, (r8)
		
END : 	br	END

DIVIDE: 	mov	r2, r4	#r2 = remainder
	movi	r5, 10	#divisor
	movi	r3, 0	#r3 quotient

CONT:	blt	r2, r5, DIV_END
	sub 	r2, r2, r5
	addi	r3, r3, 1
	br 	CONT

DIV_SEC: mov	r6, r3	#r6 = remainder
	movi	r7, 0	#r7 quotient

CONT_S:	blt	r6, r5, DIV_END
	sub 	r6, r6, r5
	addi	r7, r7, 1
	br 	CONT_S


DIV_END:		ret

N:	.word 	976
Digits:	.space	4
	
	.end 	