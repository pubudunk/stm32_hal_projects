/*
 * main.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */


#include <main.h>
#include <stdio.h>
#include <string.h>

void SystemClock_Config_HSI(enum AHB_FREQ freq);
void SystemClock_Config_HSE(enum AHB_FREQ freq);
void ErrorHandler(void);

void GPIO_Init(void);
void Timer2_Init(void);
void UART1_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htim2;	/* To input capture */

UART_HandleTypeDef huart1;

const uint32_t	uiPulseValue_500Hz	= 25000;
const uint32_t	uiPulseValue_1KHz	= 12500;
const uint32_t	uiPulseValue_2KHz	= 6250;
const uint32_t	uiPulseValue_4KHz	= 3125;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	SystemClock_Config_HSE(AHB_FREQ_50MHZ);

	/* Init timer 2 */
	Timer2_Init();

	/* External clock out */
	GPIO_Init();

	/* Init UART1 */
	UART1_Init();

//	ret = HAL_UART_Transmit(&huart1, (uint8_t*)pcUserData, strlen(pcUserData), HAL_MAX_DELAY);
//	if(ret != HAL_OK) {
//		/* Error happened. Trap */
//		ErrorHandler();
//	}

	/* Start Timers */


	ret = HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	ret = HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	ret = HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_3);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	ret = HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_4);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	for(;;) {}

	return 0;
}

void SystemClock_Config_HSI(enum AHB_FREQ freq)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;
	uint32_t FLatency;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0, sizeof(ClkInit));

	/* Configure and Enable HSI */
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	OscInit.HSIState = RCC_HSI_ON;
	OscInit.HSICalibrationValue = 16;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.HSEState = RCC_HSE_OFF;
	OscInit.LSIState = RCC_LSI_OFF;
	OscInit.PLL.PLLState = RCC_PLL_ON;
	OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(freq)
	{
	case AHB_FREQ_50MHZ:
		OscInit.PLL.PLLM = 8;	/* HSI 16MHz To get AHB CLK 50MHz M=8, N=50, P=2 */
		OscInit.PLL.PLLN = 50;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_1WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_84MHZ:
		OscInit.PLL.PLLM = 8;	/* HSI 16MHz To get AHB CLK 84MHz M=8, N=84, P=2 */
		OscInit.PLL.PLLN = 84;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_2WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_120MHZ:
		OscInit.PLL.PLLM = 8;	/* HSI 16MHz To get AHB CLK 120MHz M=8, N=120, P=2 */
		OscInit.PLL.PLLN = 120;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

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
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
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

void SystemClock_Config_HSE(enum AHB_FREQ freq)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;
	uint32_t FLatency;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0, sizeof(ClkInit));

	/* Configure and Enable HSE */
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscInit.HSIState = RCC_HSI_OFF;
	OscInit.HSICalibrationValue = 0;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.HSEState = RCC_HSE_ON;
	OscInit.LSIState = RCC_LSI_OFF;
	OscInit.PLL.PLLState = RCC_PLL_ON;
	OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(freq)
	{
	case AHB_FREQ_50MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 50MHz M=8, N=50, P=2 */
		OscInit.PLL.PLLN = 50;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_1WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_84MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 84MHz M=8, N=84, P=2 */
		OscInit.PLL.PLLN = 84;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_2WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_120MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 120MHz M=8, N=120, P=2 */
		OscInit.PLL.PLLN = 120;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

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

		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 180MHz M=8, N=180, P=2 */
		OscInit.PLL.PLLN = 180;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
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

void Timer2_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	TIM_OC_InitTypeDef sTim2OCConfig = {0};

	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	//htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim2.Init.Period = 0xFFFFFFFF;		/* Use max 32 bit range */
	htim2.Init.Prescaler = 1; 			/* Device the clock by two = 25MHz */

	ret = HAL_TIM_OC_Init(&htim2);
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	sTim2OCConfig.OCMode = TIM_OCMODE_TOGGLE;	/* Generate square wave */
	sTim2OCConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sTim2OCConfig.Pulse = uiPulseValue_500Hz;		/* 25000 pulses from 25MHz for 500Hz sq wave toggle */
	ret = HAL_TIM_OC_ConfigChannel(&htim2, &sTim2OCConfig, TIM_CHANNEL_1);
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	sTim2OCConfig.Pulse = uiPulseValue_1KHz;
	ret = HAL_TIM_OC_ConfigChannel(&htim2, &sTim2OCConfig, TIM_CHANNEL_2);
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	sTim2OCConfig.Pulse = uiPulseValue_2KHz;
	ret = HAL_TIM_OC_ConfigChannel(&htim2, &sTim2OCConfig, TIM_CHANNEL_3);
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	sTim2OCConfig.Pulse = uiPulseValue_4KHz;
	ret = HAL_TIM_OC_ConfigChannel(&htim2, &sTim2OCConfig, TIM_CHANNEL_4);
	if(ret != HAL_OK) {
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

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef ClkOutGPIO;
	ClkOutGPIO.Mode = GPIO_MODE_OUTPUT_PP;
	ClkOutGPIO.Pull = GPIO_NOPULL;
	ClkOutGPIO.Pin = GPIO_PIN_8;
	ClkOutGPIO.Alternate = 0;
	ClkOutGPIO.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &ClkOutGPIO);
}

uint32_t uiCCRRegVal = 0;

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* 500 Mhz output */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
		uiCCRRegVal = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1,uiCCRRegVal + uiPulseValue_500Hz);
	}

	/* 1KHz output signal */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
		uiCCRRegVal = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2,uiCCRRegVal + uiPulseValue_1KHz);
	}

	/* 2KHz output signal */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
		uiCCRRegVal = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3,uiCCRRegVal + uiPulseValue_2KHz);
	}

	/* 4KHz output signal */
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) {
		uiCCRRegVal = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4,uiCCRRegVal + uiPulseValue_4KHz);
	}
}
