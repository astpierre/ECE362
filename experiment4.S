// ECE 362 
// LAB EXPERIMENT 4
// Tuesday, 2/06/18

#Directives
  		.syntax unified
  		.cpu cortex-m0
  		.fpu softvfp
	  	.thumb

#Equates
		.equ	NVIC, 0xE000E100
		.equ 	ISER, 0x00
		.equ	ICER, 0x80
		.equ	ISPR, 0x100
		.equ	ICPR, 0x180
		.equ	IPR0, 0x300
		.equ	IPR1, 0x304
		.equ	IPR2, 0x308
		.equ	IPR3, 0x30C
		.equ	IPR4, 0x310
		.equ	IPR5, 0x314
		.equ	IPR6, 0x318
		.equ	IPR7, 0x31C

		.equ	RCC, 0x40021000
		.equ	AHBENR, 0x14
		.equ	APB1ENR, 0X1c

		.equ	TIM2, 0x40000000
		.equ	CR1, 0x00
		.equ	CR2, 0x04
		.equ	SMCR, 0x08
		.equ	DIER, 0x0C
		.equ	SR, 0x10
		.equ	EGR, 0x14
		.equ	CCMR1, 0x18
		.equ	CCMR2, 0x1C
		.equ	CCER, 0x20
		.equ	CNT, 0x24
		.equ	PSC, 0x28
		.equ	ARR, 0x2C
		.equ	CCR1, 0x34
		.equ	CCR2, 0x38
		.equ	CCR3, 0x3C
		.equ	CCR4, 0x40
		.equ	DCR, 0x48
		.equ	DMAR, 0x4C

		.equ	GPIOA, 0x48000000
		//.equ	GPIOB,	 #Insert system base address here
		.equ	GPIOC, 0x48000800
		//.equ	GPIOD,	 #Insert system base address here
		//.equ	GPIOF,	 #Insert system base address here
		.equ	MODER, 0x00
		.equ	OTYPER, 0x04
		.equ	OSPEEDR, 0x08
		.equ	PUPDR, 0x0C
		.equ	IDR, 0x10
		.equ	ODR, 0x14
		.equ	BSRR, 0x18
		.equ	LCKR, 0x1C
		.equ	AFRL, 0x20
		.equ	AFRH, 0x24
		.equ	BRR, 0x28

.global	main
.global TIM2_ISR

main:
		nop
gpioConfig:
	#perform GPIO configurations here
	#Activate GPIOC Clock in RCC Config
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

	#Activate GPIOA Clock in RCC Config
	ldr r5, =RCC
	ldr r6, =AHBENR
	ldr r0, [r5, r6]
	ldr r1, =0x000A0000
	orrs r0, r0, r1
	str r0, [r5, r6]

	#Config PC0 as input
	ldr r5, =GPIOA
	ldr r6, =MODER
	ldr r0, [r5, r6]
	ldr r1, =0x00000000
	orrs r0, r0, r1
	str r0, [r5, r6]
tim2Config:
		#perform TIM2 configurations here
		#Config RCC
		ldr r5, =RCC
		ldr r6, =APB1ENR
		ldr r0, [r5, r6]
		ldr r1, =0x00000001
		orrs r0, r0, r1
		str r0, [r5, r6]

		#Config Timer2 CR1
		ldr r5, =TIM2
		ldr r6, =CR1
		ldr r0, [r5, r6]
		ldr r1, =0x00
		orrs r0, r0, r1
		str r0, [r5, r6]

		#Config Timer2 PSC
		ldr r5, =TIM2
		ldr r6, =PSC
		ldr r0, [r5, r6]
		ldr r1, =48000
		str r1, [r5, r6]

		#Config Timer2 ARR
		ldr r5, =TIM2
		ldr r6, =ARR
		ldr r0, [r5, r6]
		ldr r1, =10 //5ms timer
		str r1, [r5, r6]

		#Enable Timer2 update interrupt
		ldr r5, =TIM2
		ldr r6, =DIER
		ldr r0, [r5, r6]
		ldr r1, =0x01
		orrs r0, r0, r1
		str r0, [r5, r6]

		#Enable Timer2
		ldr r5, =TIM2
		ldr r6, =CR1
		ldr r0, [r5, r6]
		ldr r1, =0x01
		orrs r0, r0, r1
		str r0, [r5, r6]

nvicConfig:
		#perform NVIC configurations here
		#Enable Timer2 interrupt in NVIC
		ldr r5, =NVIC
		ldr r6, =ISER
		ldr r0, [r5, r6]
		ldr r1, =0x00008000
		orrs r0, r0, r1
		str r0, [r5, r6]

		#Enable Timer2 interrupt in NVIC icpr
		ldr r5, =NVIC
		ldr r6, =ISPR
		ldr r0, [r5, r6]
		ldr r1, =0x00008000
		orrs r0, r0, r1
		str r0, [r5, r6]
infiniteLoop:
		nop
		ldr r0,=buttonPress
		ldr r0,[r0]
		cmp r0,#1
		bne infiniteLoop
		#button has been pressed
		movs r0,#0
		ldr r1,=buttonPress
		str r0,[r1]
startCounter:
	ldr r5,=GPIOC
	ldr r6,=ODR
	ldr r0,[r5,r6]
	ldr r1,=0x00000000
	ands r0,r0,r1
	str r0,[r5,r6]


reactionLoop:
	ldr r0,=msCount
	ldr r0,[r0]
	ldr r1,=5000
	ldr r0,[r1]
	bgt timeoutCheck

	ldr r5,=GPIOC
	ldr r6,=BSRR
	ldr r0,[r5,r6]
	ldr r1,=0x00000100
	orrs r0,r0,r1
	str r0,[r5,r6]
timeoutCheck:
	ldr r0,=msCount
	ldr r0,[r6]
	cmp r0,#0
	beq defeat

	ldr r0,=buttonPress
	ldr r0,[r0]
	cmp r0,#0
	beq reactionLoop

	movs r0,#0
	ldr r1, =buttonPress
	str r0,[r1]

	ldr r0,=msCount
	ldr r0,[r0]
	ldr r1,=5500
	cmp r0,r1

	bgt defeat
	ldr r1,=4500
	cmp r0,r1
	blt defeat
victory:
	ldr r5,=GPIOC
	ldr r6,=BSRR
	ldr r0,[r5,r6]
	ldr r1,=0x00000200
	orrs r0,r0,r1
	str r0,[r5,r6]

victoryLoop:
		ldr r0, =buttonPress
		ldr r0,[r0]
		cmp r0,#1
		bne victoryLoop
		movs r0,#0
		ldr r1,=buttonPress
		str r0,[r1]
		b startCounter
defeat:
	movs r2,#4
defeatLoop1:
		ldr r0,=500
		ldr r1,=msCount
		str r0,[r1]

		ldr r5,=GPIOC
		ldr r6,=ODR
		ldr r0,[r5,r6]
		ldr r1,=0x00000200
		eors r0,r0,r1
		str r0,[r5,r6]
defeatLoop2:
		ldr r0, =msCount
		ldr r0, [r0]
		movs r1, #0
		cmp r0, r1
		bne defeatLoop2
		subs r2,r2,#1
		cmp r2,r1
		bne defeatLoop1

defeatLoop3:
		ldr r0, =buttonPress
		ldr r0, [r0]
		cmp r0, #1
		bne defeatLoop3
		movs r0, #0
		ldr r1, =buttonPress
		str r0, [r1]
		b	 startCounter

TIM2_ISR:
		#Clear Timer2 update interrupt flag (UIF)
		ldr r6, =TIM2
		ldr r6, =SR
		ldr r0, [r5, r6]
		ldr r1, =0xFFFFFFFE
		ands r0, r0, r1
		str r0, [r5, r6]

		#Decrement ms counter
		ldr r0, =msCount
		ldr r0, [r0]
		cmp r0, #0
		beq buttonDebounce
		subs r0, r0, #5
		ldr r1, =msCount
		str r0, [r1]
buttonDebounce:
		#Check button press
		ldr r5,=GPIOA
		ldr r6,=IDR
		ldr r0,[r5,r6]
		movs r1,#0x01
		ands r0,r1,r0
		movs r2,r0
		beq isrReturn

		ldr r0,=buttonState0
		ldr r0,[r0]
		cmp r0,#0
		bne isrReturn
		movs r0,#1
		ldr r1,=buttonPress
		str r0,[r1]
isrReturn:
		ldr r1,=buttonState0
		str r2,[r1]
		bx	 lr

		.data
		.align 4
msCount:	  .word 0
buttonPress:  .word 0
buttonState0: .word 0
