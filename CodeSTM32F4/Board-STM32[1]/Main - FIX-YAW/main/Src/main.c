
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
#include "MPU9250.h"
#include "P2P_Algorithm.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "math.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//char data[120];
float t_1 = 10.851287 ; //10.839892;
float t_2 = 106.651787 ; //106.670067; 
//float t_3 =  10.851229 ; //10.85124 ;
//float t_4 =  106.651794; //106.651808;
//10.851287, 106.651787

//10.851229, 106.651794

float				z;
float			z1;
float				x;
float			x1;
double 			disp2p;
double angle=0;
int st; 
//char data_receive[22]={0};
char data_receive[15];
char data_receive_2;
//double bearing_;
//double kc;
//move_to MOVE;
//float z2 = 10.839890; //10.839892, 106.670067 // 35m
//float x2 = 106.670098 ;

//float z2 = 10.838784; //10.839892, 106.670067 // 35m
//float x2 = 106.671440  ;

char data_send[21];
double z2,x2; //truyen tu C#
MY_MPU9250	IMU;
float roll,	pitch,	yaw,yaw_heading;
float time;
int _i1;

int32_t xung_l = 0;
int32_t xung_r = 0;
uint8_t state_r = 0 ; // trang thai hien tai cua encoder
uint8_t state_l = 0;
//bien tam gan cho encoder
uint8_t _a=0;
uint8_t _b=0;
uint8_t _c=0;
uint8_t _d=0;
uint8_t prestate_r = 0; // trang thai truoc cua encoder
uint8_t prestate_l = 0;
//xung cho timer2 pwm CH2-CH4
int32_t pulse_l = 0;
int32_t pulse_r = 0;
//pwm cho CH2 - CH4
int16_t duty_l = 0;
int16_t duty_r = 0;
float data_x=0.0f,data_y=0.0f,test;
float new_delta_angel;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

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
  MX_I2C1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	//right
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
	//left
	HAL_Delay(100);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
	GPS_Init(&huart3);

	MPU9250_Init_All(&hi2c1);
//	MPU9250_Read_All(&hi2c1);
	MPU9250_Calib_Gyro(&hi2c1);
	MPU9250_Calib_Mag(&hi2c1);
	
	
	//HAL_UART_Receive_DMA(&huart2,(uint8_t*)data_receive,sizeof(data_receive));
	//HAL_UART_Transmit(&huart2,(uint8_t*)data_send,sizeof(data_send),1000);	
	//HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
	
	HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
	memset(&data_receive,'0',sizeof(data_receive));
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	HAL_Delay(100);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_Delay(10);
	HAL_TIM_Base_Start_IT(&htim2); //for send data
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		//MPU9250_Get_Angles(&hi2c1,&yaw,&pitch,&roll);
		
//		
//		data_receive_2 = data_receive[0];
//		sscanf(data_receive,"%c %f %f",&data_receive_2,&data_x,&data_y);
//		HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
			data_receive_2 = data_receive[0];
			sscanf(data_receive,"%c %f %f",&data_receive_2,&data_x,&data_y);
			HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
			
			
		
		//yaw_heading=-yaw+180.0f;		
//		HAL_Delay(2000);
//			//duty_l = PID_L(40,pulse_l);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
//	//	duty_r = PID_R(40,pulse_r);
//			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
		
		
	//	angle = bearing_Angle(z,x,z2,x2);
		//disp2p = distanceBetween(z,x,z2,x2); // chay dc 
		
		//angle = bearing_Angle(z,x,t_3,t_4);
		//disp2p = distanceBetween(z,x,t_3,t_4); // chay dc 
		
			
//		if(z== 0 || x==0){
//			angle = 0;
//			disp2p = 0;
//		}
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

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 9;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 419;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim4);

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

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
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
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9|GPIO_PIN_11, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE7 PE9 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE10 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD9 PD11 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PD0 PD1 PD3 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	if (huart->Instance==huart3.Instance)
//	{
//		GPS_CallBack(&huart3);
//	}
//  if (huart->Instance==huart2.Instance)
//	{
//		st=1;
//		HAL_UART_Receive_DMA(&huart2,(uint8_t*)data_receive,sizeof(data_receive));
//		z2 = _getLat2(data_receive);
//		x2 = _getLong2(data_receive);
//		//_i1 = 0;
//		//memset(&data_send,0,sizeof(data_send));
//		HAL_UART_Transmit(&huart2,(uint8_t*)data_send,strlen(data_send),1000);	
//		//HAL_Delay(1000);
//	//	HAL_UART_Transmit(&huart2,(uint8_t*)data_send,sizeof(data_send),1000);	
//		
//			//HAL_UART_Transmit(&huart1,(uint8_t*)data_send,strlen(data_send),1000);
//	
//	}
	if (huart->Instance==huart6.Instance)
	{
			
			sscanf(data_receive,"%c %f %f",&data_receive_2,&data_x,&data_y);
			HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
				if(data_receive_2 == '0')
			{
				data_x = 0.0f;
				data_y = 0.0f;
				//data_receive[]={'0'};
				memset(&data_receive,0,sizeof(data_receive));
			
			}
		
	}
} 

void TIM3_IRQHandler(void)
{
	//MPU9250_Get_Angles(&hi2c1,&yaw,&pitch,&roll);
	//MPU9250_Angles_Update(&hi2c1,&IMU);
	MPU9250_Get_Angles(&hi2c1,&yaw,&pitch,&roll);
	
		//_i1++;
//	if(_i1 == 300){
//			sprintf(data_send,"%.6f %.6f\r\n",t_1,t_2); //FOR TEST
//			//sprintf(data_send,"%0.6f %0.6f\r\n",z,x);
//			HAL_UART_Transmit(&huart2,(uint8_t*)data_send,sizeof(data_send),1000);	
//			_i1=0;
//	}
//		if(data_receive[0] == '0'){
//			data_x = 0.0f;
//			data_y = 0.0f;
//			memset(&data_receive,'0',sizeof(data_receive));
//		}
//	_i1++;
//	data_receive_2 = data_receive[0];
//		if(_i1 >= 100){
//					
//				sscanf(data_receive,"%c %f %f",&data_receive_2,&data_x,&data_y);
//				HAL_UART_Receive(&huart6,(uint8_t*)data_receive,sizeof(data_receive),1000);
//			_i1=0;
//	}
	
		//if (yaw < 0) yaw += 360;
		// this command do not edit and put it in the last line of this void
	HAL_TIM_IRQHandler(&htim3);
}
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
	
//	_i1++;
//	if(_i1 == 100){
//			sprintf(data_send,"%.6f %.6f\r\n",t_1,t_2); //FOR TEST
//			//sprintf(data_send,"%0.6f %0.6f\r\n",z,x);
//			HAL_UART_Transmit(&huart2,(uint8_t*)data_send,sizeof(data_send),1000);	
//			_i1=0;
//	}
	pulse_l = xung_l;
	xung_l = 0; // reset lai encoder left
	pulse_r = xung_r;
	xung_r = 0; // reset lai encoder right
	if(data_x>180.0f){
		yaw_heading=-yaw+180.0f;		
	}
	else {
		yaw_heading=-yaw+9.183f;
	}
//	new_delta_angel = fabs (data_x - yaw_heading);	if(new_delta_angel <20){
//		duty_l = PID_L(32.5,pulse_l);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, duty_l);
//		duty_r = PID_R(33,pulse_r);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty_r);
//	}
//	if(data_y < 5.0f) {
//		//duty_l = PID_L(40,pulse_l);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, 0);
//	//	duty_r = PID_R(40,pulse_r);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0);
//	}
//	

	
//		duty_l = PID_L(32.5,pulse_l);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, duty_l);
//		duty_r = PID_R(33,pulse_r);
//		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, duty_r);

	
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

void EXTI3_IRQHandler(void)
{
	
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)==GPIO_PIN_SET)
		_a=1;
	else
		_a=0;
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)==GPIO_PIN_SET)
		_b=1;
	else
		_b=0;
	state_l=(state_l<<1)|_a;
	state_l=(state_l<<1)|_b;
	state_l=state_l&0x03;
	switch (state_l)
		{
		case 0:
			if(prestate_l==1) xung_l++;
			else xung_l--;
		break;
		case 1:
			if(prestate_l==3) xung_l++;
			else xung_l--;
		break;
			case 2:
			if(prestate_l==0) xung_l++;
			else xung_l--;
			break;
			case 3:
			if(prestate_l==2) xung_l++;
			else xung_l--;
			break;
		}
	prestate_l=state_l;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
void EXTI4_IRQHandler(void)
{
	
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3)==GPIO_PIN_SET)
		_a=1;
	else
		_a=0;
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4)==GPIO_PIN_SET)
		_b=1;
	else
		_b=0;
	state_l=(state_l<<1)|_a;
	state_l=(state_l<<1)|_b;
	state_l=state_l&0x03;
	switch (state_l)
		{
		case 0:
			if(prestate_l==1) xung_l++;
			else xung_l--;
		break;
		case 1:
			if(prestate_l==3) xung_l++;
			else xung_l--;
		break;
			case 2:
			if(prestate_l==0) xung_l++;
			else xung_l--;
			break;
			case 3:
			if(prestate_l==2) xung_l++;
			else xung_l--;
			break;
		}
	prestate_l=state_l;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

//right wheel
//PD0 PD1 Encoder banh phai

//---for right wheel---------
void EXTI0_IRQHandler(void)
{
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)==GPIO_PIN_SET)
		_c=1;
	else
		_c=0;
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1)==GPIO_PIN_SET)
		_d=1;
	else
		_d=0;
	state_r=(state_r<<1)|_d;
	state_r=(state_r<<1)|_c;
	state_r=state_r&0x03;
	switch (state_r)
		{
		case 0:
			if(prestate_r==1) xung_r++;
			else xung_r--;
		break;
		case 1:
			if(prestate_r==3) xung_r++;
			else xung_r--;
		break;
			case 2:
			if(prestate_r==0) xung_r++;
			else xung_r--;
			break;
			case 3:
			if(prestate_r==2) xung_r++;
			else xung_r--;
			break;
		}
	prestate_r=state_r;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
void EXTI1_IRQHandler(void)
{
	
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0)==GPIO_PIN_SET)
		_c=1;
	else
		_c=0;
	if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1)==GPIO_PIN_SET)
		_d=1;
	else
		_d=0;
	state_r=(state_r<<1)|_d;
	state_r=(state_r<<1)|_c;
	state_r=state_r&0x03;
	switch (state_r)
		{
		case 0:
			if(prestate_r==1) xung_r++;
			else xung_r--;
		break;
		case 1:
			if(prestate_r==3) xung_r++;
			else xung_r--;
		break;
			case 2:
			if(prestate_r==0) xung_r++;
			else xung_r--;
			break;
			case 3:
			if(prestate_r==2) xung_r++;
			else xung_r--;
			break;
		}
	prestate_r=state_r;
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
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
