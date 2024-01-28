/*
 * main.c
 *
 *  Created on: Jan 27, 2024
 *      Author: pubudukarunaratna
 */


#include <main.h>
#include <stdio.h>
#include <string.h>

void SystemClock_Config_HSE(enum AHB_FREQ freq);
void ErrorHandler(void);

void UART1_Init(void);
void Log_Message(const char *pcMessage);

void CAN1_Init(void);
void CAN1_TX(void);
void CAN1_Filter_Config(void);

UART_HandleTypeDef huart1;

CAN_HandleTypeDef hcan1;

int main(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Init HAL */
	ret = HAL_Init();
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	SystemClock_Config_HSE(AHB_FREQ_50MHZ);

	/* Init UART1 */
	UART1_Init();

	Log_Message("CAN Loopback Interrupt Test\n");

	CAN1_Init();

	CAN1_Filter_Config();			/* Configure before move to normal mode */


	ret = HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF);		/* Activate interrupt */
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	ret = HAL_CAN_Start(&hcan1);	/* Move the CAN controller to Normal mode */
	if(ret != HAL_OK) {
		ErrorHandler();
	}

	CAN1_TX();

	for(;;) {}

	return 0;
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

void ErrorHandler(void)
{
	for(;;)
	{
		/* Error handle code */
	}
}

void UART1_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* Set the Base address of USASRT 1 base address */
	huart1.Instance = USART1;

	/* Set USART1 communication params */
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
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

void CAN1_Init(void)
{
	HAL_StatusTypeDef ret = 0;

	memset(&hcan1, 0, sizeof(hcan1));

	hcan1.Instance = CAN1;
	hcan1.Init.Mode = CAN_MODE_LOOPBACK;
	hcan1.Init.AutoBusOff = DISABLE;
	hcan1.Init.AutoRetransmission = ENABLE;
	hcan1.Init.AutoWakeUp = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;

	/* CAN bit timing */
	hcan1.Init.Prescaler = 5;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;

	ret = HAL_CAN_Init( &hcan1 );	/* Move the CAN controller to initialization mode and param init */
	if( ret != HAL_OK ) {
		ErrorHandler();
	}
}

void CAN1_TX(void)
{
	CAN_TxHeaderTypeDef sTxHdr = {0};
	uint32_t ulTxMailBox = 0;
	HAL_StatusTypeDef ret = 0;

	const uint8_t ucMsg[] = {'I','D',' ','6','5'};

	sTxHdr.IDE = CAN_ID_STD;	/* Standard CAN frame */
	sTxHdr.DLC = 5;				/* 5 byte to tx */
	sTxHdr.StdId = 0x65;
	sTxHdr.RTR = CAN_RTR_DATA;	/* data frame */

	ret = HAL_CAN_AddTxMessage( &hcan1, &sTxHdr, ucMsg, &ulTxMailBox );
	if( ret != HAL_OK ) {
		ErrorHandler();
	}
}

void CAN1_Filter_Config(void)
{
	HAL_StatusTypeDef ret = 0;
	CAN_FilterTypeDef sCAN1FilterID = {0};

#if 0
	CAN_FilterTypeDef sCAN1FilterAcceptAll = {0};
	sCAN1FilterAcceptAll.FilterActivation =  ENABLE;
	sCAN1FilterAcceptAll.FilterBank = 0;
	sCAN1FilterAcceptAll.FilterFIFOAssignment = CAN_RX_FIFO0;
	sCAN1FilterAcceptAll.FilterIdHigh = 0x00;		/* Accept all */
	sCAN1FilterAcceptAll.FilterIdLow = 0x00;
	sCAN1FilterAcceptAll.FilterMaskIdHigh = 0x00;
	sCAN1FilterAcceptAll.FilterMaskIdLow = 0x00;
	sCAN1FilterAcceptAll.FilterMode = CAN_FILTERMODE_IDMASK;
	sCAN1FilterAcceptAll.FilterScale = CAN_FILTERSCALE_32BIT;
#endif

	sCAN1FilterID.FilterActivation =  ENABLE;
	sCAN1FilterID.FilterBank = 0;
	sCAN1FilterID.FilterFIFOAssignment = CAN_RX_FIFO0;
	sCAN1FilterID.FilterIdHigh = (0x65 << 5);		/* Accept ID 0x65 */
	sCAN1FilterID.FilterIdLow = 0x00;
	sCAN1FilterID.FilterMaskIdHigh = 0x00;
	sCAN1FilterID.FilterMaskIdLow = 0x00;
	sCAN1FilterID.FilterMode = CAN_FILTERMODE_IDLIST;
	sCAN1FilterID.FilterScale = CAN_FILTERSCALE_32BIT;

	ret = HAL_CAN_ConfigFilter(&hcan1, &sCAN1FilterID);
	if( ret != HAL_OK ) {
		ErrorHandler();
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	Log_Message("Message Transmitted MB0\n");
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	Log_Message("Message Transmitted MB1\n");
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	Log_Message("Message Transmitted MB2\n");
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	HAL_StatusTypeDef ret = 0;
	CAN_RxHeaderTypeDef sRxHdr = {0};
	uint8_t ucRxBuf[5] = {0};

	ret = HAL_CAN_GetRxMessage( &hcan1, CAN_RX_FIFO0, &sRxHdr, ucRxBuf );
	if( ret != HAL_OK ) {
		ErrorHandler();
	}

	Log_Message("CAN Message Received: ");
	Log_Message((const char*)ucRxBuf);
	Log_Message("\n");
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	char cMsg[100] = {0};

	snprintf(cMsg, sizeof(cMsg), "CAN Error: %lX\n", hcan->ErrorCode);
	Log_Message(cMsg);
}


