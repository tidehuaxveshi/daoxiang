#include "can_com.h"
can_tx_t txcan_1 = {0};
can_rx_t rxcan_1 = {0};
can_tx_t txcan_2 = {0};
can_rx_t rxcan_2 = {0};
can_tx_t txcan_3 = {0};
can_rx_t rxcan_3 = {0};

can_tx_t txcan_test = {0};
can_rx_t rxcan_test = {0};
void fdcan_setting_init(FDCAN_HandleTypeDef *hfdcan, uint32_t offset)
{
	hfdcan->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan->Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan->Init.AutoRetransmission = DISABLE;
	hfdcan->Init.TransmitPause = DISABLE;
	hfdcan->Init.ProtocolException = DISABLE;
	hfdcan->Init.NominalPrescaler = 8;
	hfdcan->Init.NominalSyncJumpWidth = 1;
	hfdcan->Init.NominalTimeSeg1 = 5;
	hfdcan->Init.NominalTimeSeg2 = 2;
	hfdcan->Init.DataPrescaler = 1;
	hfdcan->Init.DataSyncJumpWidth = 1;
	hfdcan->Init.DataTimeSeg1 = 1;
	hfdcan->Init.DataTimeSeg2 = 1;
	hfdcan->Init.MessageRAMOffset = offset;
	hfdcan->Init.StdFiltersNbr = 8;
	hfdcan->Init.ExtFiltersNbr = 8;
	hfdcan->Init.RxFifo0ElmtsNbr = 8;
	hfdcan->Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan->Init.RxFifo1ElmtsNbr = 0;
	hfdcan->Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
	hfdcan->Init.RxBuffersNbr = 0;
	hfdcan->Init.RxBufferSize = FDCAN_DATA_BYTES_8;
	hfdcan->Init.TxEventsNbr = 0;
	hfdcan->Init.TxBuffersNbr = 0;
	hfdcan->Init.TxFifoQueueElmtsNbr = 8;
	hfdcan->Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	hfdcan->Init.TxElmtSize = FDCAN_DATA_BYTES_8;
	if (HAL_FDCAN_Init(hfdcan) != HAL_OK)
	{
		Error_Handler();
	}
}

void can_two_group_tx_update(can_tx_t *tx, uint32_t id_1, uint32_t id_2, uint8_t length, uint32_t STD_EXT, uint32_t RTR)
{
	tx->header_group1.Identifier = id_1;
	tx->header_group1.IdType = STD_EXT;
	tx->header_group1.TxFrameType = RTR;
	tx->header_group1.DataLength = length;
	tx->header_group1.BitRateSwitch = FDCAN_BRS_OFF;
	tx->header_group1.FDFormat = FDCAN_CLASSIC_CAN;
	tx->header_group1.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	tx->header_group1.TxEventFifoControl = FDCAN_NO_TX_EVENTS;

	tx->header_group2.Identifier = id_2;
	tx->header_group2.IdType = STD_EXT;
	tx->header_group2.TxFrameType = RTR;
	tx->header_group2.DataLength = length;
	tx->header_group2.BitRateSwitch = FDCAN_BRS_OFF;
	tx->header_group2.FDFormat = FDCAN_CLASSIC_CAN;
	tx->header_group2.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	tx->header_group2.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
}

void can_rx_mask_update(can_rx_t *rx, uint32_t id, uint32_t mask_setting, uint32_t fifo_setting, uint32_t filter_bank, uint32_t STD_EXT, uint32_t RTR)
{
	rx->filter_config.FilterIndex = filter_bank;
	rx->filter_config.IdType = STD_EXT;
	rx->filter_config.FilterType = FDCAN_FILTER_MASK;

	rx->filter_config.FilterID1 = id;
	rx->filter_config.FilterID2 = mask_setting;
	HAL_FDCAN_ConfigFilter(rx->can_channel, &rx->filter_config);
}

void can_send_data_two(can_tx_t *tx)
{
	HAL_FDCAN_AddMessageToTxFifoQ(tx->can_channel, &tx->header_group1, tx->data_group1);
	 HAL_FDCAN_AddMessageToTxFifoQ(tx->can_channel, &tx->header_group2, tx->data_group2);
}

void can_init(void)
{
	fdcan_setting_init(COMMUNICATION_CAN_1, 0);
	fdcan_setting_init(COMMUNICATION_CAN_2, 512);
	fdcan_setting_init(COMMUNICATION_CAN_3, 1024);

	txcan_1.can_channel = COMMUNICATION_CAN_1;
	rxcan_1.can_channel = COMMUNICATION_CAN_1;
	txcan_2.can_channel = COMMUNICATION_CAN_2;
	rxcan_2.can_channel = COMMUNICATION_CAN_2;
	txcan_3.can_channel = COMMUNICATION_CAN_3;
	rxcan_3.can_channel = COMMUNICATION_CAN_3;


	// 111 1111 0000:0x200-0x20f
	can_two_group_tx_update(&txcan_1, RM_GROUP_1, RM_GROUP_2, RM_DATA_SIZE_TRANSMIT, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);
	can_rx_mask_update(&rxcan_1, 0x0, 0x0, FDCAN_FILTER_TO_RXFIFO0, 0, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);

	can_two_group_tx_update(&txcan_2, RM_GROUP_1, RM_GROUP_2, RM_DATA_SIZE_TRANSMIT, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);
	can_rx_mask_update(&rxcan_2, 0x0, 0x0, FDCAN_FILTER_TO_RXFIFO0, 0, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);

	can_two_group_tx_update(&txcan_3, RM_GROUP_1, RM_GROUP_2, RM_DATA_SIZE_TRANSMIT, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);
	can_rx_mask_update(&rxcan_3, 0x0, 0x0, FDCAN_FILTER_TO_RXFIFO0, 0, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);

	HAL_FDCAN_Start(COMMUNICATION_CAN_1);
	HAL_FDCAN_Start(COMMUNICATION_CAN_2);
	HAL_FDCAN_Start(COMMUNICATION_CAN_3);

	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO1_NEW_MESSAGE);
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	// __HAL_CAN_ENABLE_IT(COMMUNICATION_CAN, CAN_IT_TX_MAILBOX_EMPTY);
}

// void HAL_FDCAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hfdcan) // 报文接收中断
// {
// 	HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rxcan.rx_header, rxcan.data);
// }
