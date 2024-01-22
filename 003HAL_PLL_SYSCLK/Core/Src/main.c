/*
 * main.c
 *
 *  Created on: Jan 19, 2024
 *      Author: pubudukarunaratna
 */



#include <main.h>
#include <stdio.h>
#include <string.h>


enum AHB_FREQ {
	AHB_FREQ_120MHZ=0,
	AHB_FREQ_180MHZ
};


void UART1_Init(void);
void ErrorHandler(void);
void SystemClock_Config(enum AHB_FREQ freq);

UART_HandleTypeDef huart1;



int main(void)
{
	char msg[100];

	/* Init HAL */
	HAL_Init();

	SystemClock_Config(AHB_FREQ_180MHZ);

	/* Init UART1 After changing clocks*/
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

void SystemClock_Config(enum AHB_FREQ freq)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;
	uint32_t FLatency;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0, sizeof(ClkInit));

	/* Configure and Enable HSE */
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	OscInit.HSIState = RCC_HSI_ON;
	OscInit.HSICalibrationValue = 16;
	OscInit.HSEState = RCC_HSE_OFF;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.LSIState = RCC_LSI_OFF;
	OscInit.PLL.PLLState = RCC_PLL_ON;
	OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(freq)
	{
	case AHB_FREQ_120MHZ:
		OscInit.PLL.PLLM = 8;	/* HSI 16MHz To get AHB CLK 120MHz M=8, N=120, P=2 */
		OscInit.PLL.PLLN = 120;
		OscInit.PLL.PLLP = 2;
		OscInit.PLL.PLLQ = 2;

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_3WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_180MHZ:

		/* Enable the clock for the power controller */
		__HAL_RCC_PWR_CLK_ENABLE();

		/* regulator voltage scale 1 */
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		/* Enable the over drive mode */
		__HAL_PWR_OVERDRIVE_ENABLE();

		OscInit.PLL.PLLM = 8;	/* HSI 16MHz To get AHB CLK 180MHz M=8, N=180, P=2 */
		OscInit.PLL.PLLN = 180;
		OscInit.PLL.PLLP = 2;
		OscInit.PLL.PLLQ = 2;

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_5WS;	/* Set the correct flash latency */

		break;
	default:
		break;
	}

	ret = HAL_RCC_OscConfig(&OscInit);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}


	ret = HAL_RCC_ClockConfig(&ClkInit, FLatency);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	/* Configure SYSTICK */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
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


