/*
 * main.h
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */

#ifndef INC_MAIN_H_
#define INC_MAIN_H_

#include "stm32f4xx_hal.h"

enum AHB_FREQ {
	AHB_FREQ_50MHZ=0,
	AHB_FREQ_84MHZ,
	AHB_FREQ_120MHZ,
	AHB_FREQ_180MHZ
};

#define TRUE	1
#define FALSE	0

#endif /* INC_MAIN_H_ */
