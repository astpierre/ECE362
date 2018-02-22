// ECE 362 
// LAB EXPERIMENT 3
// Tuesday, 1/30/18

	.syntax unified
	.cpu cortex-m0
	.fpu softvfp
	.thumb
#Equates
	.equ RCC, 0x40021000
	.equ AHBENR, 0x14
	.equ GPIOC, 0x48000800
	.equ GPIOB, 0x48000400
	.equ GPIOA, 0x48000000
	.equ MODER, 0x00
	.equ OTYPER, 0x04
	.equ ODR, 0x14
	.equ IDR, 0x10
	/*
	.equ	OTYPER,	 #Add register offset here
	.equ	OSPEEDR, #Add register offset here
	.equ	PUPDR,	 #Add register offset here
	.equ	IDR,	 #Add register offset here
	.equ	ODR,	 #Add register offset here
	.equ	BSRR,	 #Add register offset here
	.equ	LCKR,	 #Add register offset here
	.equ	AFRL,	 #Add register offset here
	.equ	AFRH,	 #Add register offset here
	.equ	BRR,	 #Add register offset here
	*/
.global main
main:
	nop
outputConfig:
	#Activate GPIOC Clock in RCC Configuration
	ldr r5, =RCC
	ldr r6, =AHBENR
	ldr r0, [r5, r6]
	ldr r1, =0x00080000
	orrs r0, r0, r1
	str r0, [r5, r6]
	#Configure PC8 as output
	ldr r5, =GPIOC
	ldr r6, =MODER
	ldr r0, [r5, r6]
	ldr r1, =0x00010000
	orrs r0, r0, r1
	str r0, [r5, r6]
	#Configure PC9 as output
	ldr r5, =GPIOC
	ldr r6, =MODER
	ldr r0, [r5, r6]
	ldr r1, =0x00040000
	orrs r0, r0, r1
	str r0, [r5, r6]
inputConfig:
	#Activate GPIOA Clock in RCC Configuration
	ldr r5, =RCC
	ldr r6, =AHBENR
	ldr r0, [r5, r6]
	ldr r1, =0x000A0000
	orrs r0, r0, r1
	str r0, [r5, r6]
getPBstatus:
	#Configure PA0 as input
	ldr r5, =GPIOA
	ldr r6, =MODER
	ldr r0, [r5, r6]
	ldr r1, =0x00000000
	orrs r0, r0, r1
	str r0, [r5, r6]
readPB:
	ldr r5, =GPIOA
	ldr r6, =IDR
	ldr r0, [r5, r6]
	ldr r1, =0x00000001
	ands r0, r0, r1
	ldr r3,=0x1
	cmp r0,r3
	bne blueOn
	beq greenOn
blueOn:
	#target blue
	ldr r5, =GPIOC
	ldr r6, =ODR
	ldr r1, =0x00000100
	str r1, [r5, r6]
	b 	readPB
greenOn:
	#target green
	ldr r5, =GPIOC
	ldr r6, =ODR
	ldr r1, =0x00000200
	str r1,[r5,r6]
	b 	readPB
