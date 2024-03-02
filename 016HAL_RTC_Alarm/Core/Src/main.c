/*
 * main.c
 *
 *  Created on: Jan 21, 2024
 *      Author: pubudukarunaratna
 */


#include <main.h>
#include <stdio.h>
#include <string.h>


void ErrorHandler(void);
void GPIO_Init(void);
void UART1_Init(void);
void SystemClock_Config_HSE(enum AHB_FREQ freq);
void RTC_Init(void);
void RTC_Calandar_Config(void);
void RTC_Alarm_Config(void);

void Log_Message(const char *pcMessage);

UART_HandleTypeDef huart1;
RTC_HandleTypeDef hrtc;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	SystemClock_Config_HSE(AHB_FREQ_50MHZ);

	/* enable user button interrupt */
	GPIO_Init();

	/* Init UART1 */
	UART1_Init();

	/* Init RTC */
	RTC_Init();

	Log_Message("RTC Alarm Application\r\n");

	for(;;)
	{

	}

	return 0;
}

void UART1_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Set the Base address of USASRT 1 base address */
	huart1.Instance = USART1;

	/* Set USART1 communication params */
	huart1.Init.BaudRate = 460800;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	ret = HAL_UART_Init(&huart1);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}
}

void Log_Message(const char *pcMessage)
{
	HAL_StatusTypeDef ret = 0;
	char pcUsrMsg[100];

	snprintf( pcUsrMsg, sizeof(pcUsrMsg), "%s",pcMessage );
	HAL_UART_Transmit( &huart1, (uint8_t *)pcUsrMsg, strlen(pcUsrMsg), HAL_MAX_DELAY );
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}
}

void ErrorHandler(void)
{
	for(;;)
	{
		/* Error handle code */
	}
}

void SystemClock_Config_HSE(enum AHB_FREQ freq)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	RCC_OscInitTypeDef OscInit;
	RCC_ClkInitTypeDef ClkInit;
	uint32_t FLatency;

	memset(&OscInit, 0, sizeof(OscInit));
	memset(&ClkInit, 0, sizeof(ClkInit));

	/* Configure and Enable HSE */
	OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	OscInit.HSIState = RCC_HSI_OFF;
	OscInit.HSICalibrationValue = 0;
	OscInit.LSEState = RCC_LSE_OFF;
	OscInit.HSEState = RCC_HSE_ON;
	OscInit.LSIState = RCC_LSI_OFF;
	OscInit.PLL.PLLState = RCC_PLL_ON;
	OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	switch(freq)
	{
	case AHB_FREQ_50MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 50MHz M=8, N=50, P=2 */
		OscInit.PLL.PLLN = 50;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_1WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_84MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 84MHz M=8, N=84, P=2 */
		OscInit.PLL.PLLN = 84;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;		/* APB1 25Mhz */
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;

		FLatency = FLASH_ACR_LATENCY_2WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_120MHZ:
		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 120MHz M=8, N=120, P=2 */
		OscInit.PLL.PLLN = 120;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;	/* This is not used. Just use the default config value */

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_3WS;	/* Set the correct flash latency */

		break;
	case AHB_FREQ_180MHZ:

		/* Enable the clock for the power controller */
		__HAL_RCC_PWR_CLK_ENABLE();

		/* regulator voltage scale 1 */
		__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

		/* Enable the over drive mode */
		__HAL_PWR_OVERDRIVE_ENABLE();

		OscInit.PLL.PLLM = 4;	/* HSI 16MHz To get AHB CLK 180MHz M=8, N=180, P=2 */
		OscInit.PLL.PLLN = 180;
		OscInit.PLL.PLLP = RCC_PLLP_DIV2;
		OscInit.PLL.PLLQ = 2;

		/* Configure SYSCLK */

		ClkInit.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
				| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
		ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
		ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
		ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
		ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

		FLatency = FLASH_ACR_LATENCY_5WS;	/* Set the correct flash latency */

		break;
	default:
		break;
	}

	ret = HAL_RCC_OscConfig(&OscInit);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}


	ret = HAL_RCC_ClockConfig(&ClkInit, FLatency);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	/* Configure SYSTICK */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI0_IRQn, 14, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void RTC_Init(void)
{
	int ret = HAL_OK;

	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 249;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	ret = HAL_RTC_Init(&hrtc);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

}

void RTC_Calandar_Config(void)
{
	/* Setting TIme- 12:15:00 PM  Date- 1 March 2024 Friday */
	RTC_TimeTypeDef stRTCTime = {0};
	RTC_DateTypeDef stRTCDate = {0};

	stRTCTime.Hours = 12;
	stRTCTime.Minutes = 15;
	stRTCTime.Seconds = 00;
	stRTCTime.TimeFormat = RTC_HOURFORMAT12_AM;
	HAL_RTC_SetTime(&hrtc, &stRTCTime, RTC_FORMAT_BIN);

	stRTCDate.Date = 1;
	stRTCDate.Month = RTC_MONTH_MARCH;
	stRTCDate.Year = 24;
	stRTCDate.WeekDay = RTC_WEEKDAY_FRIDAY;
	HAL_RTC_SetDate(&hrtc, &stRTCDate, RTC_FORMAT_BIN);
}

void RTC_Calandar_Config_1(void)
{
	/* Setting TIme- 7:59:50 AM Date- 3 March 2024 Sunday */
	RTC_TimeTypeDef stRTCTime = {0};
	RTC_DateTypeDef stRTCDate = {0};

	stRTCTime.Hours = 7;
	stRTCTime.Minutes = 59;
	stRTCTime.Seconds = 50;
	stRTCTime.TimeFormat = RTC_HOURFORMAT12_AM;
	HAL_RTC_SetTime(&hrtc, &stRTCTime, RTC_FORMAT_BIN);

	stRTCDate.Date = 3;
	stRTCDate.Month = RTC_MONTH_MARCH;
	stRTCDate.Year = 24;
	stRTCDate.WeekDay = RTC_WEEKDAY_SUNDAY;
	HAL_RTC_SetDate(&hrtc, &stRTCDate, RTC_FORMAT_BIN);
}

void RTC_Alarm_Config(void)
{
	int ret = HAL_OK;
	RTC_AlarmTypeDef stRTCAlarmA = {0};
	/* Set alarm xx:45:09 */

	/* De-activate any active alarms. May be not required */
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	stRTCAlarmA.Alarm = RTC_ALARM_A;
	stRTCAlarmA.AlarmTime.Minutes = 45;
	stRTCAlarmA.AlarmTime.Seconds = 9;
	stRTCAlarmA.AlarmMask = (RTC_ALARMMASK_HOURS | RTC_ALARMMASK_DATEWEEKDAY); /* hours and day/date don't care */
	stRTCAlarmA.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	ret = HAL_RTC_SetAlarm_IT(&hrtc, &stRTCAlarmA, RTC_FORMAT_BIN);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	Log_Message("RTC Alarm config complete\r\n");
}

void RTC_Alarm_Config_2(void)
{
	int ret = HAL_OK;
	RTC_AlarmTypeDef stRTCAlarmA = {0};
	/* Set alarm xx:45:09 */

	/* De-activate any active alarms. May be not required */
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	stRTCAlarmA.Alarm = RTC_ALARM_A;
	stRTCAlarmA.AlarmTime.Hours = 12;
	stRTCAlarmA.AlarmTime.Minutes = 15;
	stRTCAlarmA.AlarmTime.Seconds = 15;
	stRTCAlarmA.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	stRTCAlarmA.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY; /* day/date don't care */
	stRTCAlarmA.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	ret = HAL_RTC_SetAlarm_IT(&hrtc, &stRTCAlarmA, RTC_FORMAT_BIN);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	Log_Message("RTC Alarm config complete\r\n");
}

void RTC_Alarm_Config_3(void)
{
	int ret = HAL_OK;
	RTC_AlarmTypeDef stRTCAlarmA = {0};
	/* Set alarm xx:45:09 */

	/* De-activate any active alarms. May be not required */
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	stRTCAlarmA.Alarm = RTC_ALARM_A;
	stRTCAlarmA.AlarmTime.Hours = 8;
	stRTCAlarmA.AlarmTime.Minutes = 0;
	stRTCAlarmA.AlarmTime.Seconds = 0;
	stRTCAlarmA.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;
	stRTCAlarmA.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	stRTCAlarmA.AlarmDateWeekDay = RTC_WEEKDAY_SUNDAY;
	stRTCAlarmA.AlarmMask = RTC_ALARMMASK_NONE; // RTC_ALARMMASK_SECONDS; // If seconds was masked interrupt is generated every second after other fields are matched
	stRTCAlarmA.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;

	ret = HAL_RTC_SetAlarm_IT(&hrtc, &stRTCAlarmA, RTC_FORMAT_BIN);
	if(ret != HAL_OK) {
		/* Error happened. Trap */
		ErrorHandler();
	}

	Log_Message("RTC Alarm config complete\r\n");
}

char* GetDayofWeek(uint8_t day)
{
	char *weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

	return weekday[day-1];
}

void RTC_Print_Date_Time(void)
{
	char buffer[100] = {0};
	RTC_TimeTypeDef stRTCTime = {0};
	RTC_DateTypeDef stRTCDate = {0};

	HAL_RTC_GetTime(&hrtc, &stRTCTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &stRTCDate, RTC_FORMAT_BIN);

	snprintf(buffer, sizeof(buffer), "Current Time is: %02d:%02d:%02d %s\r\n", stRTCTime.Hours,
			stRTCTime.Minutes, stRTCTime.Seconds, (stRTCTime.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM" );
	Log_Message(buffer);

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "Current Date is: %02d-%02d-%02d %s\r\n", stRTCDate.Month,
			stRTCDate.Date, stRTCDate.Year, GetDayofWeek(stRTCDate.WeekDay));
	Log_Message(buffer);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//RTC_Calandar_Config();
	RTC_Calandar_Config_1(); /* For Sunday alarm */


	Log_Message("RTC Time set\r\n");
	RTC_Print_Date_Time();

	//RTC_Alarm_Config();
	//RTC_Alarm_Config_2();
	RTC_Alarm_Config_3();	/* For Sunday alarm */
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	Log_Message("RTC Alarm A triggered\r\n");
	RTC_Print_Date_Time();
}
