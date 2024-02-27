/*
 * main.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */


#include <main.h>
#include <stdio.h>
#include <string.h>


void ErrorHandler(void);
void Timer7_Init(void);
void GPIO_Init(void);
void UART1_Init(void);

void Log_Message(const char *pcMessage);


TIM_HandleTypeDef htim7;

UART_HandleTypeDef huart1;

char uart_data[] = "We are testing SLEEPONEXIT\r\n";

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	/* Init Timer6 */
	Timer7_Init();

	TIM7->SR = 0;

	/* Init UART1 */
	UART1_Init();

	/* Enable Sleep on exit */
	HAL_PWR_EnableSleepOnExit();

	/* Start timer Interrupt mode */
	HAL_TIM_Base_Start_IT(&htim7);

	for(;;) {}

	return 0;
}

void Timer7_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	htim7.Instance = TIM7;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP; /* Only available mode for basic timer */
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim7.Init.Prescaler = 4999;
	htim7.Init.Period = 32-1;


	ret = HAL_TIM_Base_Init(&htim7);
	if(ret != HAL_OK) {
		ErrorHandler();
	}
}

void UART1_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Set the Base address of USASRT 1 base address */
	huart1.Instance = USART1;

	/* Set USART1 communication params */
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	ret = HAL_UART_Init(&huart1);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}
}

void Log_Message(const char *pcMessage)
{
	HAL_StatusTypeDef ret = 0;
	char pcUsrMsg[100];

	snprintf( pcUsrMsg, sizeof(pcUsrMsg), "%s",pcMessage );
	HAL_UART_Transmit( &huart1, (uint8_t *)pcUsrMsg, strlen(pcUsrMsg), HAL_MAX_DELAY );
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}
}

void ErrorHandler(void)
{
	for(;;)
	{
		/* Error handle code */
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	Log_Message(uart_data);
}
