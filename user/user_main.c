#include "user_main.h"

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if (hfdcan == COMMUNICATION_CAN_1)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_1.rx_header, rxcan_1.data);
	}
	else if (hfdcan == COMMUNICATION_CAN_2)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_2.rx_header, rxcan_2.data);
	}
	else if (hfdcan == COMMUNICATION_CAN_3)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan_3.rx_header, rxcan_3.data);
	}


    data_extract(&wheel,&rxcan_3);
}
void tim_init(void)
{
	__HAL_TIM_SET_PRESCALER
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);
}
void user_init(void)
{
	can_init();
	serial_init();
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM1)
	{
		serial_voltage_output_test_dma(&transmit_packet);
	}
	else if( htim->Instance == TIM8)
	{
		current_adjust_all(&wheel, 1.5);
		current_set(&wheel, &txcan_1);
		HAL_Delay(1000);
		current_set(&wheel, &txcan_2);
		HAL_Delay(1000);
		current_set(&wheel, &txcan_3);
		HAL_Delay(1000);
	}
}