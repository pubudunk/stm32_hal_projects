/*
 * it.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */
#include <main.h>

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();
}

extern TIM_HandleTypeDef htim7;

void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);
}
