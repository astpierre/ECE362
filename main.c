/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    04-March-2018
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"
volatile int red_duty = 4096;
volatile int green_duty = 4096;
volatile int blue_duty = 4096;
volatile int count = 1;
volatile int up = 1;
volatile int sample_adc = 10;
volatile int adc_reading1;
volatile int color = 0;
volatile int next_color;
volatile int btnCurrent;
volatile int pressed;
volatile int btnPrev;
int main(void)
{
		//GPIOA CONFIG ALT
		RCC   ->AHBENR  |= RCC_AHBENR_GPIOAEN;//enable clock to GPIOA
		GPIOA -> MODER  |= 0x002A0000;//pin PA8,PA9,PA10 = Alt Fun
		GPIOA -> AFR[1] |= 0x00000222;//pin PA8,9,10 = TIM1_CH123

		RCC  -> AHBENR  |= 0x00080000;//enable clock to GPIOC
		GPIOC -> MODER  |= 0x00000015;//pin PC0,1,2 output

		//TIM1 CONFIG
		RCC  -> APB2ENR |= RCC_APB2ENR_TIM1EN;//enable timer 1 peripheral
		TIM1 -> CR1      = 0x00;//timer disabled
		TIM1 -> BDTR    |= 0x8000;// break and dead time
		TIM1 -> ARR      = 0x00001000;//auto reload value
		TIM1 -> DIER    |= 0x01;//
		TIM1 -> CCER    |= 0x0111;//capture compare enable
		TIM1 -> CCMR1   |= 0x7070;//PWM Mode 2 (inactive until TIM1_CNT > TIM1_CCRx)
		TIM1 -> CCMR2   |= 0x7070;
		TIM1 -> CCR1     = red_duty;//duty cycle
		TIM1 -> CCR2     = green_duty;//duty cycle
		TIM1 -> CCR3     = blue_duty;//duty cycle

		TIM1 -> CR1     |= 0x01;//timer enabled

		//TIM2 CONFIG
		RCC -> APB1ENR |= 0x00000001;
		TIM2 -> CR1     = 0x00;
		TIM2 -> PSC     = 7999;
		TIM2 -> ARR     = 4;
		TIM2 -> DIER   |= 0x01;
		TIM2 -> CR1    |= 0x01;

		//ADC CONFIG
		/* (1) enable clock to ADC GPIOA pin */
		/* (2) PA0 as analog input pin */
		/* (3) Enable the peripheral clock of the ADC */
		/* (4) Start HSI14 RC oscillator */
		/* (5) Wait HSI14 is ready */
		/* (6) Enable the ADC */
		/* (7) Wait until ADC ready */
		//RCC   ->AHBENR  |= RCC_AHBENR_GPIOAEN;//enable clock to GPIOB
		GPIOA -> MODER |=  0x0000003C; 				// (2) (should be 0x0000000C)
		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 		// (3)
		RCC->CR2 |= RCC_CR2_HSI14ON; 				// (4)
		while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0); // (5)
		ADC1->CR |= ADC_CR_ADEN; 					// (6)
		while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); 	// (7)
		while((ADC1->CR & ADC_CR_ADSTART) == 1);
		ADC1->CHSELR = 2;

		//INTERRUPT ENABLE
		NVIC -> ISER[0] |= 0x00008000;

		//INFINITE LOOP
		while(1)
		{
			//TIM1 -> CCR1     = duty_cycle;//duty cycle
			//TIM1 -> CCR2     = duty_cycle;//duty cycle
			//TIM1 -> CCR3     = duty_cycle;//duty cycle
			if (color == 0)//BLUE
			{
				TIM1 -> CCR1     = blue_duty;//duty cycle
				TIM1 -> CCR2     = green_duty;//duty cycle
				TIM1 -> CCR3     = red_duty;//duty cycle

				GPIOC -> ODR = 0x1;//R
				next_color = color + 1;
			}
			else if (color == 1)//GREEN
			{
				TIM1 -> CCR1     = blue_duty;//duty cycle
				TIM1 -> CCR2     = green_duty;//duty cycle
				TIM1 -> CCR3     = red_duty;//duty cycle

				GPIOC -> ODR = 0x2;//B
				next_color = color + 1;
			}
			else if (color == 2)//RED
			{
				TIM1 -> CCR1     = blue_duty;//duty cycle
				TIM1 -> CCR2     = green_duty;//duty cycle
				TIM1 -> CCR3     = red_duty;//duty cycle

				GPIOC -> ODR = 0x4;//G
				next_color =  0;
			}

		}
}
extern void TIM2_IRQHandler()
{
	TIM2 -> SR &= 0xFFFFFFFE;
	if (sample_adc == 0)
	{
		if (color == 0)
		{
			blue_duty = adcRead();
		}
		else if (color == 1)
		{
			red_duty = adcRead();
		}
		else if (color == 2)
		{
			green_duty = adcRead();
		}
		sample_adc = 10;
	}
	else
	{
		sample_adc = sample_adc - 1;
	}
	btnCurrent = GPIOA -> IDR;
	btnCurrent = btnCurrent & 0x00000001;
	if (btnCurrent == 0x01 && btnPrev == 0)
	{
		color = next_color;
	}
	btnPrev = btnCurrent;
	return;
}
int adcRead(void)
{
		uint32_t adcValue = 0;
		// Wait for ADC to be ready
		while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
		// Start the ADC (ADCStart = 1)
		ADC1->CR |= ADC_CR_ADSTART;
		// Wait for end of conversion
		while((ADC1->ISR & ADC_ISR_EOC) == 0);
		adcValue = ADC1->DR;
		return adcValue;
}



