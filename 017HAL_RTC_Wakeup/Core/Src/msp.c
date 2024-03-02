/*
 * msp.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */
#include <main.h>

extern void ErrorHandler(void);

void HAL_MspInit(void)
{
	/* 1. Set up the priority grouping of the arm cortex m4 processor */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* 2. Enable the required system exceptions of the arm cortex m4 processor */
	/* Enable Mem-Manage, Bus fault and Usage fault exceptions */
	SCB->SHCSR = ((0x01 << 18) | (0x01 << 17) | (0x01 << 16));

	/* 3. Configure the priority for the system exceptions */
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart1;

	/* 1. Enable the clock for UART1 and GPIOA peripheral */
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_SLEEP_DISABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* 2. Do the pin muxing configuration */

	gpio_uart1.Pin = GPIO_PIN_9;			/* USART1_TX */
	gpio_uart1.Mode = GPIO_MODE_AF_PP;
	gpio_uart1.Pull = GPIO_NOPULL;
	gpio_uart1.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart1.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &gpio_uart1);

	gpio_uart1.Pin = GPIO_PIN_10;			/* USART1_RX */
	HAL_GPIO_Init(GPIOA, &gpio_uart1);

	/* Enable IRQ and setup priority (NVIC settings ) (optional to use in interrupt mode) */
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);	/* Set to lowest priority */
}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	int ret = HAL_OK;

	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_PeriphCLKInitTypeDef RCC_RTCPeriClkInit = {0};

	/* 1. Turn on LSI */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	/* Select LSI as RTCCLK */
	RCC_RTCPeriClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	RCC_RTCPeriClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
	ret = HAL_RCCEx_PeriphCLKConfig(&RCC_RTCPeriClkInit);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	/* 3. Enable RCC clock */
	__HAL_RCC_RTC_ENABLE();

	/* Enable RTC Alarm IRQ */
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
