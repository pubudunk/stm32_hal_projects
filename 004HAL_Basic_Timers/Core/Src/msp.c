/*
 * msp.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */
#include "main.h"

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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htimer)
{
	/* Enable TIM7 clock */
	__HAL_RCC_TIM7_CLK_ENABLE();

	/* Enable Tim 7 enable */
	HAL_NVIC_EnableIRQ(TIM7_IRQn);

	HAL_NVIC_SetPriority(TIM7_IRQn, 15, 0);
}
