/*
 * power_modes.h
 *
 *  Created on: Feb 28, 2024
 *      Author: pubudukarunaratna
 */

#ifndef INC_POWER_MODES_H_
#define INC_POWER_MODES_H_

void PWR_StopMainRegFlashOn(void);
void PWR_StopMainRegFlashPwrDn(void);
void PWR_StopLPRegFlashOn(void);
void PWR_StopLPRegFlashPwrDn(void);
void PWR_StopMainRegUnderDrvFlashPwrDn(void);
void PWR_StopLPRRegUnderDrvFlashPwrDn(void);

#endif /* INC_POWER_MODES_H_ */
