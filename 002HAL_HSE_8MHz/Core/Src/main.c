/*
 * main.c
 *
 *  Created on: Jan 19, 2024
 *      Author: pubudukarunaratna
 */



#include "main.h"
#include <stdio.h>
#include <string.h>

void UART1_Init(void);
void ErrorHandler(void);


UART_HandleTypeDef huart1;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;

	char msg[100];

	/* Init HAL */
	HAL_Init();

	/* Init UART1 */
	UART1_Init();

	/* Configure and Enable HSE */
	memset(&OscInit, 0, sizeof(OscInit));
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscInit.HSEState = RCC_HSE_ON;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.HSIState = RCC_HSI_OFF;
	OscInit.HSICalibrationValue = 0;
	OscInit.LSIState = RCC_LSI_OFF;
	ret = HAL_RCC_OscConfig(&OscInit);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	/* Configure SYSCLK */
	memset(&ClkInit, 0, sizeof(ClkInit));
	ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV2;
	ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;
	ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;
	ret = HAL_RCC_ClockConfig(&ClkInit, FLASH_ACR_LATENCY_0WS);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	__HAL_RCC_HSI_DISABLE(); /* Save current */

	/* Configure SYSTICK */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* Re-Init UART1 After changing clocks*/
	UART1_Init();

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), "SYSCLK: %ld\n", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), "HCLK: %ld\n", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), "PCLK1: %ld\n", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	memset(msg, 0, sizeof(msg));
	snprintf(msg, sizeof(msg), "PCLK2: %ld\n", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


	while(1) {}

	return 0;
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

void ErrorHandler(void)
{
	for(;;)
	{
		/* Error handle code */
	}
}


