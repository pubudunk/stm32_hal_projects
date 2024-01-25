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
void GPIO_Init(void);
void SystemClock_Config(enum AHB_FREQ freq);
void Timer2_Init(void);
void HSE_MCO_Config(void);
void UART1_Init(void);
void Timer7_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htim2;	/* To input capture */
TIM_HandleTypeDef htim7;	/* To generate external clock */

static uint32_t inputCaptures[2] = {0};
static uint8_t ucCount  = 1;
static uint8_t isCaptureComplete = FALSE;

UART_HandleTypeDef huart1;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	uint32_t captureDifference;
	double tim2CntFreq, userSigTimePeriod, userSigFreq;
	char pcUserData[100];

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	SystemClock_Config(AHB_FREQ_50MHZ);

	/* Init timer 2 */
	Timer2_Init();

#if 0
	/* Configure HSE Clock out */
	HSE_MCO_Config();

	memset(pcUserData, 0, sizeof(pcUserData));
	snprintf(pcUserData, sizeof(pcUserData), "Timer 2 Input Capture - Capture input HSE(8Mhz) / 4 (2MHz)\n");
	/* 2Mhz clock generated clock period is 0.5 us but the TIM IRQ handler takes 5.5 us to complete. Processor hangs and cannot proceed. */
#else
	/* Init Timer7 - External clock */
	Timer7_Init();

	/* External clock out */
	GPIO_Init();

	/* Start timer Interrupt mode */
	HAL_TIM_Base_Start_IT(&htim7);

	memset(pcUserData, 0, sizeof(pcUserData));
	snprintf(pcUserData, sizeof(pcUserData), "Timer 2 Input Capture - Capture input External 50Khz gen from tim7\n");
#endif

	/* Start Timer 2 Capture */
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

	/* Init UART1 */
	UART1_Init();

	ret = HAL_UART_Transmit(&huart1, (uint8_t*)pcUserData, strlen(pcUserData), HAL_MAX_DELAY);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	for(;;) {
		if(TRUE == isCaptureComplete) {
			if(inputCaptures[1] > inputCaptures[0])
				captureDifference = inputCaptures[1] - inputCaptures[0];
			else
				captureDifference = 0xFFFFFFFF - (inputCaptures[0] - inputCaptures[1]);

			tim2CntFreq = 2 * HAL_RCC_GetPCLK1Freq() / (htim2.Init.Prescaler + 1);
			userSigTimePeriod = captureDifference * (1 / tim2CntFreq);
			userSigFreq = 1 / userSigTimePeriod;

			memset(pcUserData, 0, sizeof(pcUserData));
			snprintf(pcUserData, sizeof(pcUserData), "User signal frequency = %f\n", userSigFreq);

			ret = HAL_UART_Transmit(&huart1, (uint8_t*)pcUserData, strlen(pcUserData), HAL_MAX_DELAY);
			if(ret != HAL_OK) {
				/* Error happened. Trap */
				ErrorHandler();
			}

			isCaptureComplete = FALSE;

		}


	}

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

	/* Configure and Enable HSI */
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
	OscInit.HSIState = RCC_HSI_ON;
	OscInit.HSICalibrationValue = 16;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.HSEState = RCC_HSE_ON;
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

void HSE_MCO_Config(void)
{
	/* Get the clock out from MCO1 (PA8). CLK out will be 8Mhz/4 - 2Mhz */
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_4); 	/* Function is doing GPIO config inside. No need to do again */
}

void Timer2_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	TIM_IC_InitTypeDef tim2ICConfig;

	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim2.Init.Period = 0xFFFFFFFF;		/* Use max 32 bit range */
	htim2.Init.Prescaler = 1; 			/* Device the clock by two */

	ret = HAL_TIM_IC_Init(&htim2);
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	tim2ICConfig.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2ICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2ICConfig.ICPrescaler = TIM_ICPSC_DIV1;
	tim2ICConfig.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&htim2, &tim2ICConfig, TIM_CHANNEL_1);
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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(FALSE == isCaptureComplete)
	{
		if(1 == ucCount) {
			inputCaptures[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			ucCount++;
		} else if(2 == ucCount) {
			inputCaptures[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			ucCount = 1;
			isCaptureComplete = TRUE;
		}
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

void Timer7_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Generate 50Khz Clock */
	htim7.Instance = TIM7;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP; /* Only available mode for basic timer */
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim7.Init.Prescaler = 9;
	htim7.Init.Period = 50-1;


	ret = HAL_TIM_Base_Init(&htim7);
	if(ret != HAL_OK) {
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
}
