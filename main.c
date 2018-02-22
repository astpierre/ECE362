/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    02/13/2018
  * @brief   Default main function. In this experiment, students will learn 
			 basics of writing embedded firmware using higher-level languages, and
			 how the various constructs used in the assembly-based 
			 labs translate over into C.
  ******************************************************************************
*/
#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"
			
volatile int user_pb;
volatile int last_state;
volatile int pressed;
volatile int button_prev = 0;
volatile int letsgo;
volatile int countdown;
volatile int playing;
volatile int next_step;
volatile int react;
/*
int main(void)
{
		//STEPS 1+2
		RCC -> AHBENR |=  0x00080000;
		GPIOC -> MODER |= 0x00040000;
		GPIOC -> MODER |= 0x00010000;
		RCC -> AHBENR |=  0x000A0000;
		GPIOA -> MODER |= 0x00000000;
		while(1){
			user_pb = GPIOA -> IDR;
			user_pb = user_pb & 0x00000001;
			if (user_pb == 0x01)
				{
				GPIOC -> ODR = 0x00000200;
				}
			else
			{
				GPIOC -> ODR = 0x00000100;
			}
		}
		//return;
}
*/
//STEPS 3+4
int main(void)
{
		//GPIO CONFIG
		RCC  -> AHBENR |= 0x00080000;
		GPIOC -> MODER |= 0x00010000;
		GPIOC -> MODER |= 0x00040000;
		RCC -> AHBENR |=  0x000A0000;
		GPIOA -> MODER |= 0x00000000;

		//TIM2 CONFIG
		RCC -> APB1ENR |= 0x00000001;
		TIM2 -> CR1     = 0x00;
		TIM2 -> PSC     = 48000;
		TIM2 -> ARR     = 4;
		TIM2 -> DIER   |= 0x01;
		TIM2 -> CR1    |= 0x01;

		//INTERRUPT ENABLE
		NVIC -> ISER[0] = 0x00008000;

		//variables
		int start;
		start = 0;
		countdown = 1;
		playing = 0;
		int loser;
		int menu;
		menu = 0;
		next_step = 0;
		//INFINITE LOOP
		while(1)
		{
			//user_pb = GPIOA -> IDR;
			//user_pb = user_pb & 0x00000001;
			/*
			if (user_pb == 0x01 && pressed == 0 && playing == 0)
			{
				button_prev = 1;
			}
			*/
			if (pressed == 1 && playing == 1)
			{
				react = 1;
				//pressed = 0;

			}
			else if (pressed == 1 &&  playing == 0)
			{
				next_step = 0;
				pressed = 0;
				start = 1;
			}

			if (start == 1)
			{
				//GAME STARTS
				countdown = 840;//about 5.5s
				start = 0;
			}
			while (playing == 1)
			{
				//countdown = countdown - 1;
				GPIOC -> ODR &=  0x00000000;
				if (countdown <= 420)
				{
					GPIOC -> ODR = 0x00000100;
				}
				else
				{
					GPIOC -> ODR = 0x00000000;
				}
				if (react == 1)
				{
					if (countdown > 460 && countdown < 730)
					{
						//GPIOC -> ODR = 0x00000100;
						loser = 0;
						menu = 1;
						next_step = 1;
						break;
					}
					else
					{
						//GPIOC -> ODR = 0x00000200;
						loser = 1;
						menu = 1;
						next_step = 1;
						break;
					}
				}
				else if (next_step == 1)
				{
					loser = 1;
					menu = 1;
					next_step = 1;
					break;
				}

			}
			if (next_step == 1)
			{
				if (loser == 0 && menu == 1)
				{
					GPIOC -> ODR = 0x00000100;
					playing = 0;
				}
				else if (loser == 1 && menu == 1)
				{
					GPIOC -> ODR = 0x00000200;
					playing = 0;
				}
				//next_step = 0;
				menu = 0;
				react = 0;
				//pressed =1;
				/*
				if (user_pb == 0x01 && pressed == 0 && playing == 0)
				{
					button_prev = 1;
				}
				else if (pressed == 1 && button_prev == 0)
				{
					pressed = 0;
					start = 1;
				}
				*/


			//endgame

			//GPIOC -> ODR |=  0x00000200;
		}
}
}
extern void TIM2_IRQHandler()
	{
	TIM2 -> SR &= 0xFFFFFFFE;
	//GPIOC -> ODR ^= 0x00000100;
	user_pb = GPIOA -> IDR;
	user_pb = user_pb & 0x00000001;

	if (user_pb == 0x01 && playing ==1)
			{
				react =1;
				//pressed = 1;
				//button_prev = 0;
			}
	else if (user_pb == 0x01 && button_prev == 0)
		{
			pressed = 1;
		}

	button_prev = user_pb;

	if (countdown > 1 && next_step == 0)
	{
		countdown = countdown - 1;
		playing = 1;
	}
	else if (countdown == 0)
	{
		playing = 0;
		next_step = 1;
		countdown = 1;
	}

	return;
	}








