/*
 * msp.c
 *
 *  Created on: Jan 27, 2024
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
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);	/* Set to lowest priority */
}

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef sGPIO_CAN1 = {0};

	/* 1. Enable the clock for CAN1 and GPIOA peripheral */
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* 2. Do the pin muxing configuration */
	sGPIO_CAN1.Pin = GPIO_PIN_11 | GPIO_PIN_12;
	sGPIO_CAN1.Mode = GPIO_MODE_AF_PP;
	sGPIO_CAN1.Pull = GPIO_NOPULL;
	sGPIO_CAN1.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	sGPIO_CAN1.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOA, &sGPIO_CAN1);

	/* Enable interrupts and set priority */
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
}
