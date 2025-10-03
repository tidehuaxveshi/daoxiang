#include "can_com.h"
can_tx_t txcan = {0};
can_rx_t rxcan = {0};



void can_tx_update(can_tx_t *tx, uint32_t id, uint8_t length, uint32_t STD_EXT, uint32_t RTR)
{
	tx->header.Identifier = id;
	tx->header.IdType = STD_EXT;
	tx->header.TxFrameType = RTR;
	tx->header.DataLength = length;

	tx->header.BitRateSwitch = FDCAN_BRS_OFF;
	tx->header.FDFormat = FDCAN_CLASSIC_CAN;
	tx->header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	tx->header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
}

void can_rx_mask_update(can_rx_t *rx, uint32_t id, uint32_t mask_setting, uint32_t fifo_setting, uint32_t filter_bank, uint32_t STD_EXT, uint32_t RTR)
{
	rx->filter_config.FilterIndex = filter_bank;
	rx->filter_config.IdType = STD_EXT;
	rx->filter_config.FilterIndex = filter_bank;
	rx->filter_config.FilterType = FDCAN_FILTER_MASK;
	rx->filter_config.FilterConfig = fifo_setting;
	rx->filter_config.FilterID1 = id;
	rx->filter_config.FilterID2 = mask_setting;
	HAL_FDCAN_ConfigFilter(rx->can_channel, &rx->filter_config);

}

void CAN_Send_Data(can_tx_t *tx)
{
	HAL_FDCAN_AddMessageToTxFifoQ(tx->can_channel, &tx->header, tx->data);
}

void can_init(void)
{
	txcan.can_channel = COMMUNICATION_CAN;
	rxcan.can_channel = COMMUNICATION_CAN;
	can_tx_update(&txcan, MOTOR_ID, DATA_SIZE_TRANSMIT, FDCAN_EXTENDED_ID, FDCAN_DATA_FRAME);
	can_rx_mask_update(&rxcan, MOTOR_ID, MASK_ALL, FDCAN_FILTER_TO_RXFIFO0, 0, FDCAN_EXTENDED_ID, FDCAN_DATA_FRAME);
	HAL_FDCAN_Start(COMMUNICATION_CAN);

	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO1_NEW_MESSAGE);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
	// __HAL_CAN_ENABLE_IT(COMMUNICATION_CAN, CAN_IT_TX_MAILBOX_EMPTY);
}

void HAL_FDCAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hfdcan) // 报文接收中断
{
	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan.rx_header, rxcan.data);
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan.rx_header, rxcan.data);
}
