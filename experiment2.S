// ECE 362 
// LAB EXPERIMENT 2
// Tuesday, 1/23/18


	.syntax unified
	.cpu cortex-m0
	.fpu softvfp
	.thumb

.global		main

main:
		nop
		.text
		ldr		r0, =ops
		ldr		r2, =ops + 4
		ldr 	r3,[r0]
		ldr		r4,[r2]
		movs 	r1, #0
divideLoop:
		cmp		r3, r4
		blt		divideReturn
		subs	r3, r3, r4 //r3 is the remainder
		adds	r1, r1, #1 //r1 is the quotient
		b		divideLoop
divideReturn:
		ldr		r5, =quot //load quot address
		str		r1,[r5] // store quotient in quot address
		ldr		r6, =remain //load remain address
		str		r3,[r6] // store remainder in remain address
		b		codeSegment2
		bkpt
codeSegment2:
		ldr		r1, =tval
		ldr 	r2,[r1]
		movs	r0,#4
		cmp		r0,r2
		bhi		setHigherHigher
		bls		setHigherLower
setHigherHigher:
		ldr		r1, =higher
		ldr 	r2,[r1]
		movs	r3,#11
		str		r3,[r1]
		b		codeSegment3
		bkpt
setHigherLower:
		ldr		r1, =higher
		ldr 	r2,[r1]
		movs	r3,#00
		str		r3,[r1]
		//b		codeSegment3
		bkpt
codeSegment3:
		PUSH	{r0-r7}
		ldr		r0, =adds
		ldr		r1, =adds + 4
		ldr		r2, =adds + 8
		ldr		r3,[r0]
		ldr		r4,[r1]
		ldr		r5,[r2]
		ADDS	r6,r3,r4
		ADDS	r6,r6,r5
		//bkpt
		ldr		r7,=sum
		str		r6,[r7]
		POP		{r0-r7}
		b		codeSegment4
		bkpt
codeSegment4:
		push	{r0-r4}
		ldr		r2, =ops
		ldr		r1, =ops1
		//ldr		r1, =ops + 4
		//ldr		r2,[r0]
		//ldr		r3,[r1]
		movs		r3,#0
		movs		r0,#8

loopFor:
		cmp 	r0,r3
		bls		endFor
		ldr		r4,[r2,r3]
		str		r4,[r1,r3]
		adds	r3,r3,#4
		b		loopFor
endFor:
		bkpt
		pop		{r0-r4}
		nop
		.align 4
ops:	.word 5 /*insert variable value here*/
		.word 2 /*insert variable value here*/
		.data
ops1:	.space 4
		.space 4
		.data

tval:	.word 5
		.data
adds:	.word 1 /*insert variable value here*/
		.word 2 /*insert variable value here*/
		.word 3 /*insert variable value here*/
		.data
higher: .space 4
quot:	.space 4
remain:	.space 4
sum:	.space 4
