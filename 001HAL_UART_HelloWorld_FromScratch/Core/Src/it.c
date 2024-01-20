/*
 * it.c
 *
 *  Created on: Jan 19, 2024
 *      Author: pubudukarunaratna
 */
#include "main.h"

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();
}

extern UART_HandleTypeDef huart1;;

void USART1_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart1);
}
