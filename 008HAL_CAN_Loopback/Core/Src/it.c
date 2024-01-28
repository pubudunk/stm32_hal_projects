/*
 * it.c
 *
 *  Created on: Jan 27, 2024
 *      Author: pubudukarunaratna
 */
#include <main.h>

void SysTick_Handler(void)
{
	HAL_IncTick();

	HAL_SYSTICK_IRQHandler();
}
