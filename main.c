/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//FOR UI
int clear = 1;// initiailization state
int state;
char clear_buffer[100];
int mode;
//FOR TRANSMITTING
int i= 0;
char buffer[100];
int len;
int greeting;
int result;
//FOR RECEIVING
char Rx_indx, Rx_data[2], Rx_Buffer[100];
int flag_uart;
//for some math
int num1;
int num2;
int sum;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
//Interrupt callback routine
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//this callback routine allows the user to parse their data and submit it using a the ENTER key
	//assigns each bit of Rx_data to a char the user input
    uint8_t i;
    if (huart->Instance == USART1)  //current UART
        {
        if (Rx_indx==0) {for (i=0;i<100;i++) Rx_Buffer[i]=0;}   //clear Rx_Buffer before receiving new data

        if (Rx_data[0]!=13) //if received data different from ascii 13 (enter)
            {
            Rx_Buffer[Rx_indx++]=Rx_data[0];    //add data to Rx_Buffer
            }
        else            //if received data = 13
            {
            Rx_indx=0;
            flag_uart = 1;//transfer complete, data is ready to read
            }
        HAL_UART_Receive_IT(&huart1, Rx_data, 1);   //activate UART receive interrupt every time
        }
}
/* USER CODE END PFP */
/* USER CODE BEGIN 0 */
int get_numba(void)
{
	int num;//return int
	int prompt_num;//print string
	char buffer_2[100];//empty array for size reference
	prompt_num=sprintf(buffer_2,"Please enter a number: ");//tell user to enter a number
	HAL_UART_Transmit(&huart1, buffer_2, prompt_num, 1000);//display prompt on serial port
	//wait until number is given
	while (flag_uart == 0)
	{}//wait for uart flag
	if (flag_uart == 1)
	{
		//prompted user for number
		num = atoi (Rx_Buffer);//stdio function converts a string to an int
		flag_uart = 0;//clear flag
	}
	return num;
}
int get_mode(void)
{
	int m;//return int
	int prompt_mode;//print string
	char buffer_3[100];//empty array for size reference
	prompt_mode=sprintf(buffer_3,"Specify an operator (+,-,*,/) or 'c' to clear: ");//tell user to select mode
	HAL_UART_Transmit(&huart1, buffer_3, prompt_mode, 1000);//display prompt on serial port
	//wait until mode is given
	while (flag_uart == 0)
	{}//wait for uart flag
	if (flag_uart == 1)
	{
		//prompted user for mode
		m = (Rx_Buffer[0]);//need to assign +,-,/,*,c to meaningful values
		flag_uart = 0;//clear flag
	}
	return m;
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	//local variables
	int show_num1;
	char blank_line[100];
	int cbuff;
	int error;
	int clc_msg;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();


  /* USER CODE BEGIN 2 */
  //HAL_UART_Receive_IT(&huart1, Rx_data, 1);//activating UART Tx in interrupt mode
  HAL_UART_Receive_IT(&huart1, Rx_data, 1);//activating UART Rx in interrupt mode
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (clear == 1)
	  {
		  clear = 0;//clear flag
		  greeting=sprintf(buffer,"Calculator Mode enabled.\r\n");//tell user we are up and running
		  HAL_UART_Transmit(&huart1, buffer, greeting, 1000);//display on serial port
		  //clear all parameters
		  sum = 0;
		  num1 = 0;
		  num2 = 0;
		  mode = 0;
		  state =1;//advance state
	  }
	  if (state == 1)
	  {
		  cbuff=sprintf(blank_line,"\n");//new line
		  HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port
		  if (sum==0)
	      {
			  num1 = get_numba();//get first number
	      }
		  else
		  {
			  num1 = sum;//use prev result
		  }
		  state = 2;//advance to next state
	  }
	  else if (state == 2)
	  {
		 /*MODE TABLE:
		  * MODE = 43 : + : addition
		  * MODE = 45 : - : subtraction
		  * MODE = 47 : / : division
		  * MODE = 42 : * : multiplication
		  * MODE = 99 : C : clear (handled below)
		  */
		 cbuff=sprintf(blank_line,"\n");//new line
		 HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port
		 mode = get_mode();//get desired operation
		 if (mode==99)
		 {
			 cbuff=sprintf(blank_line,"\n");//new line
    		 HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port
			 clc_msg=sprintf(buffer,"Result cleared.\n");//error
			 HAL_UART_Transmit(&huart1, buffer, clc_msg, 1000);//display on serial port
			 cbuff=sprintf(blank_line,"\n");//new line
			 HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port
			 clear=1;
		 }
		 else
		 {
			state = 3;//advance to next state
		 }
	  }
	  else if (state == 3)
	  {
		 cbuff=sprintf(blank_line,"\n");//new line
		 HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port
		 num2 = get_numba();//get second number
		 if (mode == 47 && num2 == 0)
		 {
			 error=sprintf(buffer,"Error: cannot divide by 0.\n");//error
			 HAL_UART_Transmit(&huart1, buffer, error, 1000);//display on serial port
			 state=1;
		 }
		 else state = 4;//advance to computation state
	  }
	  else if (state == 4)
	  {
	     cbuff=sprintf(blank_line,"\n");//new line
	     HAL_UART_Transmit(&huart1, blank_line, cbuff, 1000);//display on serial port

	     if (mode==43)
	     {
	    	 sum = num1+num2;
	     }
	     else if (mode==45)
	     {
	       	 sum = num1-num2;
	     }
	     else if (mode==42)
	     {
	      	 sum = num1*num2;
	     }
	     else if (mode==47)
	     {
	    	 sum = num1/num2;
	     }
	     result=sprintf(buffer,"Result: %d\n",sum);//show result
	     HAL_UART_Transmit(&huart1, buffer, result, 1000);//display on serial port
	  	 state = 1;//advance to initial state
	  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LD4_Pin|LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD4_Pin LD3_Pin */
  GPIO_InitStruct.Pin = LD4_Pin|LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
