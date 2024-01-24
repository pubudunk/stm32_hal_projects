/*
 * main.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */


#include "main.h"
#include <string.h>

void ErrorHandler(void);
void Timer7_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef htim7;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	/* Required for LED toggle */
	GPIO_Init();

	/* Init Timer6 */
	Timer7_Init();

	/* Start timer Interrupt mode */
	HAL_TIM_Base_Start_IT(&htim7);

#if 0
	/* Start timer Polling mode */
	HAL_TIM_Base_Start(&htim7);

	while(1)
	{
		/* Loop until the update event flag is set */
		while( !(TIM7->SR & TIM_SR_UIF) );

		TIM7->SR = 0; /* Reset TIM_SR_UIF bit */
		HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
	}
#endif

	for(;;) {}

	return 0;
}

void Timer7_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	htim7.Instance = TIM7;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP; /* Only available mode for basic timer */
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	htim7.Init.Prescaler = 159; /* Divide to 100 Khz CK_CLK */
	htim7.Init.Period = 10000-1;


	ret = HAL_TIM_Base_Init(&htim7);
	if(ret != HAL_OK) {
		ErrorHandler();
	}
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOG_CLK_ENABLE();

	GPIO_InitTypeDef RedLedGPIO;
	RedLedGPIO.Mode = GPIO_MODE_OUTPUT_PP;
	RedLedGPIO.Pull = GPIO_NOPULL;
	RedLedGPIO.Pin = GPIO_PIN_14;
	RedLedGPIO.Alternate = 0;
	RedLedGPIO.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOG, &RedLedGPIO);
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
	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
}
