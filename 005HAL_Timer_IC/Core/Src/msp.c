/*
 * msp.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */
#include <main.h>

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

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2Ch1Gpio;

	/* Enable timer 2 clock */
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* Configure a gpio to behave as timer2 input channel 1 */
	tim2Ch1Gpio.Pin = GPIO_PIN_5;
	tim2Ch1Gpio.Mode = GPIO_MODE_AF_PP;
	tim2Ch1Gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2Ch1Gpio);

	/* Enable IRQ */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);		/* Set lowest priority */
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart1;

	/* 1. Enable the clock for UART1 and GPIOA peripheral */
	__HAL_RCC_USART1_CLK_ENABLE();
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
	//HAL_NVIC_EnableIRQ(USART1_IRQn);
	//HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);	/* Set to lowest priority */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	/* Enable TIM7 clock */
	__HAL_RCC_TIM7_CLK_ENABLE();

	/* Enable Tim 7 enable */
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);
}
