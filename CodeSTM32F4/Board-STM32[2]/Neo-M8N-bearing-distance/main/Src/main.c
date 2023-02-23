
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */
#include "NeoM8N.h"
#include "getChar.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "math.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
char data[120];
float				z;
float			z1;
float				x;
float			x1;
double 			disp2p=0.0f;
double angle=0.0f;
double angle_new=0.0f;

double angel_new_f=0.0f;


//char data_send[13];
char data_send[17];
char data_send_2[21];
char data_receive[21]= {0};
char data_receive_new = '0';
//double bearing_;
//double kc;
//move_to MOVE;
//float z2 = 10.839890; //10.839892, 106.670067 // 35m
//float x2 = 106.670098 ;
float z2,x2;
float t_1 = 10.851287 ; //10.839892;
float t_2 = 106.651787 ; //106.670067; 
float test = 1.0;
int st=0;
float t_3 = 10.851308 ; //10.839892;
float t_4 = 106.651938 ; //106.670067; 
int _i1;
int _i2;
//10.851347, 106.651883
//10.851308 106.651938


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
	GPS_Init(&huart1);
	
	
	HAL_UART_Receive_DMA(&huart2,(uint8_t*)data_receive,sizeof(data_receive));
	HAL_UART_Transmit(&huart6,(uint8_t*)data_send,sizeof(data_send),1000);
	HAL_Delay(100);
	HAL_UART_Transmit(&huart2,(uint8_t*)data_send_2,sizeof(data_send_2),1000);	
	HAL_TIM_Base_Start_IT(&htim2);
	//memset(&data_receive,'0',sizeof(data_receive));
  /* USER CODE BEGIN 2 */
		//HAL_UART_Receive_DMA(&huart1,(uint8_t*)data,120);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
			//	HAL_Delay(1000);
			//HAL_UART_Receive_DMA(&huart1,(uint8_t *) data,120);
		
		//HAL_UART_Receive_DMA(&huart2,(uint8_t*)data_receive,sizeof(data_receive));
		
//		GPS_Process(&huart1);
//		z1 = GPS.GNGGA.Latitude ;
//		z =GPS.GNGGA.LatitudeDecimal;
//		x1 = GPS.GNGGA.Longitude;
//		x = GPS.GNGGA.LongitudeDecimal;
		
		
		//angle = bearing_Angle(z,x,z2,x2);
		//disp2p = distanceBetween(z,x,z2,x2); // chay dc 
//			t_1 = t_1 + 0.001f; //for test 
//			t_2= t_2 + 0.001f; //for test 
//			t_3 = t_3 + 0.001f; //for test 
//			t_4= t_4 +0.001f; //for test 
			//angle = bearing_Angle(t_1,t_2,t_3,t_4);
		//	disp2p = distanceBetween(t_1,t_2,t_3,t_4); // chay dc 
			//GPS_Process(&huart1);
			//GPS_Process(&huart1);
			z1 = (GPS.GNGGA.Latitude) ;
			z =GPS.GNGGA.LatitudeDecimal*1.000000f;
			x1 = GPS.GNGGA.Longitude;
			x = GPS.GNGGA.LongitudeDecimal*1.00000f;
			angle = bearing_Angle(z,x,z2,x2);
			disp2p = distanceBetween(z,x,z2,x2); // chay dc 
			if(z2==0 || x2 ==0 || z1==0 || x1==0){
				angle =0.0f;
				disp2p=0.0f;
				angle_new = 0.0f;
				angel_new_f=0.0f;
			}
			
			//angle = bearing_Angle(z,x,t_3,t_4);
			//disp2p = distanceBetween(z,x,t_3,t_4); // chay dc 
			
	
		//sprintf(data_send_2,"%.6f %.6f\r\n",t_3,t_4); //for test 
		sprintf(data_send_2,"%.6f %.6f\r\n",z,x);
		HAL_UART_Transmit(&huart2,(uint8_t*)data_send_2,strlen(data_send_2),1000);	
		HAL_Delay(2000);
		
		
		//t_1 = t_1 + 1.10f; //for test
	//	t_2 = t_2 +1.10f; //for test
		//test = test + 1.10f; //for test 
		
		//sprintf(data_send,"%c %.6f %.6f\r\n",data_receive_new,t_1,t_2); //for test 
		//sprintf(data_send,"%c %.6f %.6f\r\n",data_receive_new,angle,disp2p); //for test 
//		sprintf(data_send,"%c %.6f %.6f\r\n",data_receive_new,angle_new,disp2p);
//	//	sprintf(data_send,"%0.6.6f,%.6f\r\n",z,x);
//		HAL_UART_Transmit(&huart6,(uint8_t*)data_send,strlen(data_send),2000);	
//		HAL_Delay(1000);
		
		
		
  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 41999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 41999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 7;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART6 init function */
static void MX_USART6_UART_Init(void)
{

  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		if (huart->Instance==huart1.Instance){
			GPS_CallBack(&huart1);
		}
		 if (huart->Instance==huart2.Instance)
	{
		st=1;
		HAL_UART_Receive_DMA(&huart2,(uint8_t*)data_receive,sizeof(data_receive));
		z2 = _getLat2(data_receive);
		x2 = _getLong2(data_receive);
		data_receive_new = data_receive[0];
		//_i1 = 0;
		//memset(&data_send,0,sizeof(data_send));
		//HAL_UART_Transmit(&huart2,(uint8_t*)data_send_2,strlen(data_send_2),1000);	
		//HAL_Delay(1000);
	//	HAL_UART_Transmit(&huart2,(uint8_t*)data_send,sizeof(data_send),1000);	
		
			//HAL_UART_Transmit(&huart1,(uint8_t*)data_send,strlen(data_send),1000);
		if(data_receive[0] == '0'){
				memset(&data_send,'0',sizeof(data_send));
			
		}
	
	}
} 

void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	_i2++;
	if(_i2>=50){
		GPS_Process(&huart1);
//		z1 = (GPS.GNGGA.Latitude) ;
//			z =GPS.GNGGA.LatitudeDecimal*1.000000f;
//			x1 = GPS.GNGGA.Longitude;
//			x = GPS.GNGGA.LongitudeDecimal*1.00000f;
		//GPS_Init(&huart1);
		
		_i2=0;
	}
		
	_i1++;
	angle_new = fabs(angle-90.0f-9.1f);
//	if(angle>180){
//		angle_new = fabs(angle-270.0f-9.1f);
//	}
//	else{
//		angle_new = fabs(angle-90.0f-9.1f);
//	}
	angel_new_f = filter_low(angle_new);
	if(z2==0 || x2 ==0 || z1==0 || x1==0){
				//angle =0.0f;
				//disp2p=0.0f;
				angel_new_f=0.0f;
				angle_new = 0.0f;
			//	memset(&data_send,'0',sizeof(data_send));
			}
	data_receive_new = data_receive[0];
	if(_i1>=100){
			//sprintf(data_send,"%c %.2f %.2f\r\n",data_receive_new,angel_new_f,disp2p);
			sprintf(data_send,"%c %.2f %.2f",data_receive_new,angel_new_f,disp2p);
		
	//	sprintf(data_send,"%0.6.6f,%.6f\r\n",z,x);
			HAL_UART_Transmit(&huart6,(uint8_t*)data_send,strlen(data_send),1000);
		_i1=0;
	}
	
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
