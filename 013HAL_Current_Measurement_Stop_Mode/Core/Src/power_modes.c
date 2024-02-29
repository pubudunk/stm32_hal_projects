

#include "stm32f4xx_hal.h"
#include "power_modes.h"


void PWR_StopMainRegFlashOn(void)
{
	/* Disable flash power down in stop mode */
	HAL_PWREx_DisableFlashPowerDown();

	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in main regulator, Entry - WFI */
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}

void PWR_StopMainRegFlashPwrDn(void)
{
	/* Disable flash power down in stop mode */
	HAL_PWREx_EnableFlashPowerDown();

	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in main regulator, Entry - WFI */
	HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}

void PWR_StopLPRegFlashOn(void)
{
	/* Disable flash power down in stop mode */
	HAL_PWREx_DisableFlashPowerDown();

	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in low power regulator, Entry - WFI */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}

void PWR_StopLPRegFlashPwrDn(void)
{
	/* Disable flash power down in stop mode */
	HAL_PWREx_EnableFlashPowerDown();

	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in low power regulator, Entry - WFI */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}

void PWR_StopMainRegUnderDrvFlashPwrDn(void)
{
	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in main regulator, Entry - WFI */
	HAL_PWREx_EnterUnderDriveSTOPMode(PWR_MAINREGULATOR_UNDERDRIVE_ON, PWR_SLEEPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}

void PWR_StopLPRRegUnderDrvFlashPwrDn(void)
{
	/* Suspend systick to avoid unexpected wakeup */
	HAL_SuspendTick();

	/* Enter stop mode in main regulator, Entry - WFI */
	HAL_PWREx_EnterUnderDriveSTOPMode(PWR_LOWPOWERREGULATOR_UNDERDRIVE_ON, PWR_SLEEPENTRY_WFI);

	/* Resume systick */
	HAL_ResumeTick();
}
