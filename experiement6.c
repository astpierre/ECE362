/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    25-February-2018
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f0xx.h"
#include "stm32f0308_discovery.h"
			
volatile int counter = 20;
volatile int adc_reading1;
volatile int adc_reading2;
volatile int btnCurrent;
volatile int pressed;
volatile int btnPrev;
volatile int paused = 0;
int main(void)
{
		//GPIO CONFIG
		RCC  -> AHBENR  |= 0x00080000;//enable clock to GPIOC
		GPIOC -> MODER  |= 0x00050155;//pin PC0,1,2,3,4 & PC8,9 output

		RCC  -> AHBENR  |= 0x00040000;//enable clock to GPIOB
		GPIOB -> MODER  |= 0x00000155;//pin PB0,1,2,3,4 output

		//TIM2 CONFIG
		RCC -> APB1ENR |= 0x00000001;
		TIM2 -> CR1     = 0x00;
		TIM2 -> PSC     = 48000;
		TIM2 -> ARR     = 4;
		TIM2 -> DIER   |= 0x01;
		TIM2 -> CR1    |= 0x01;

		//ADC CONFIG
		/* (1) enable clock to ADC GPIOA pin */
		/* (2) PA0 as analog input pin */ //why is PA1 not working???
		/* (3) Enable the peripheral clock of the ADC */
		/* (4) Start HSI14 RC oscillator */
		/* (5) Wait HSI14 is ready */
		/* (6) Enable the ADC */
		/* (7) Wait until ADC ready */
		RCC -> AHBENR  |=  0x00020000; 				// (1)
		GPIOA -> MODER |=  0x2800003C; 				// (2)


		RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; 		// (3)
		RCC->CR2 |= RCC_CR2_HSI14ON; 				// (4)
		while ((RCC->CR2 & RCC_CR2_HSI14RDY) == 0); // (5)

		ADC1->CR |= ADC_CR_ADEN; 					// (6)
		while ((ADC1->ISR & ADC_ISR_ADRDY) == 0); 	// (7)
		while((ADC1->CR & ADC_CR_ADSTART) == 1);

		//INTERRUPT ENABLE
		NVIC -> ISER[0] = 0x00008000;

		//variables
		int thresh1 = 1200;
		int thresh2 = 2000;
		int thresh3 = 2700;
		int thresh4 = 3500;


		//INFINITE LOOP
		while(1)
		{
			if (paused == 0)
			{
			if (adc_reading1 >= thresh4)
			{
				GPIOB -> ODR = ~(0x0000001F);
			}
			else if (adc_reading1 >= thresh3)
			{
				GPIOB -> ODR = ~(0x0000000F);
			}
			else if (adc_reading1 >= thresh2)
			{
				GPIOB -> ODR = ~(0x00000007);
			}
			else if (adc_reading1 >= thresh1)
			{
				GPIOB -> ODR = ~(0x00000003);
			}
			else
			{
				GPIOB -> ODR = ~(0x00000201);
			}
			if (adc_reading2 >= thresh4)
			{
				GPIOC -> ODR = ~(0x0000031F);
			}
			else if (adc_reading2 >= thresh3)
			{
				GPIOC -> ODR = ~(0x0000030F);
			}
			else if (adc_reading2 >= thresh2)
			{
				GPIOC -> ODR = ~(0x00000307);
			}
			else if (adc_reading2 >= thresh1)
			{
				GPIOC -> ODR = ~(0x00000303);
			}
			else
			{
				GPIOC -> ODR = ~(0x00000301);
			}
			}
			else
			{
				paused = paused;
				GPIOC -> ODR |= (0x00000100);
			}

		}
}
extern void TIM2_IRQHandler()
	{
	TIM2 -> SR &= 0xFFFFFFFE;
	btnCurrent = GPIOA -> IDR;
	btnCurrent = btnCurrent & 0x00000001;
	if (btnCurrent == 0x01 && btnPrev == 0 && paused == 1)
	{
		paused = 0;
	}
	else if (btnCurrent == 0x01 && btnPrev == 0 && paused == 0)
	{
		paused = 1;
	}
	else
	{
		paused = paused;
	}
	btnPrev = btnCurrent;

	counter = counter - 1;
	if (counter == 10)
	{
		int chan1 = 1;
		adcEnableChannel(chan1);
		adc_reading1 = adcRead();
	}
	else if (counter == 0)
	{
		int chan2 = 2;
		adcEnableChannel(chan2);
		adc_reading2 = adcRead();
		counter = 20;
	}
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
void adcEnableChannel(int channel)
{
	// Wait for ADC to be ready
	while((ADC1->ISR & ADC_ISR_ADRDY) == 0);
	// Ensure ADCStart = 0
	while((ADC1->CR & ADC_CR_ADSTART) == 1);
	ADC1->CHSELR = 0;
	ADC1->CHSELR |= 1 << channel;
}
