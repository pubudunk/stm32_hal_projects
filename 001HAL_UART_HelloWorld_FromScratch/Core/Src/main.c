/*
 * main.c
 *
 *  Created on: Jan 19, 2024
 *      Author: pubudukarunaratna
 */


#define USE_POLLING 	0
#if USE_POLLING == 0
	#define USE_INTERRUPT	1
#endif

#include "main.h"
#include <string.h>

void SystemClockConfig(void);
void UART1_Init(void);
void ErrorHandler(void);
uint8_t Convert2UpperCase(uint8_t ucParam);

UART_HandleTypeDef huart1;

char *pcUserData =  "Hello World From Scratch\r\n";

uint8_t ucRxData = 0;
uint8_t ucRxDataBuf[256] = {0};
uint8_t ucRxCount = 0;
uint8_t ucRxComplete  = 0;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	HAL_Init();

	/* Init PLL */
	SystemClockConfig();

	/* Init UART1 */
	UART1_Init();

	/* Transmit greeting message polling based */
	ret = HAL_UART_Transmit(&huart1, (uint8_t*)pcUserData, strlen(pcUserData), HAL_MAX_DELAY);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}


#if(USE_POLLING == 1)
	for(;;)
	{
		ret = HAL_UART_Receive(&huart1, &ucRxData, sizeof(ucRxData), HAL_MAX_DELAY);
		if(ret != HAL_OK) {
			/* Error happened. Trap */
			ErrorHandler();
		}
		else {
			/* A byte received */
			if((ucRxCount + 1) == sizeof(ucRxDataBuf)) {
				/* reached end of buffer. Force set the last byte as \n */
				ucRxData = '\n';
			}

			ucRxDataBuf[ucRxCount++] = Convert2UpperCase(ucRxData);

			if('\n' == ucRxData){
				/* End of RX. */
				ucRxComplete = 1;
			}
		}

		if(ucRxComplete) {
			/* Re TX */
			ret = HAL_UART_Transmit(&huart1, ucRxDataBuf, ucRxCount, HAL_MAX_DELAY);
			if(ret != HAL_OK) {
				/* Error happened. Trap */
				ErrorHandler();
			}

			ucRxCount = 0;
			ucRxComplete = 0;
		}
	}
#endif
#if(USE_INTERRUPT == 1)

	while(ucRxComplete != 1)
		HAL_UART_Receive_IT(&huart1, &ucRxData, sizeof(ucRxData));

	for(;;)


#endif

	return 0;
}

void SystemClockConfig(void)
{

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

inline uint8_t Convert2UpperCase(uint8_t ucParam)
{
	uint8_t ucData;

	if((ucParam >= 'a') && (ucParam <= 'z')) {
		ucData = ucParam - 32;
	} else {
		ucData = ucParam;
	}

	return ucData;
}


#if(USE_INTERRUPT == 1)
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* A byte received */
	if((ucRxCount + 1) == sizeof(ucRxDataBuf)) {
		/* reached end of buffer. Force set the last byte as \n */
		ucRxData = '\n';
	}

	ucRxDataBuf[ucRxCount++] = Convert2UpperCase(ucRxData);

	if('\n' == ucRxData){
		/* End of RX. */
		ucRxComplete = 1;
	}

	if(ucRxComplete) {
		/* Re TX */
		HAL_UART_Transmit_IT(&huart1, ucRxDataBuf, ucRxCount);

		ucRxCount = 0;
		ucRxComplete = 0;
	}
}
#endif
