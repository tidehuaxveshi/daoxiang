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

    data_extract(&wheel,&rxcan_2);
}