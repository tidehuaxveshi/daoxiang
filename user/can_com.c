#include "can_com.h"
can_tx_t txcan_1 = {0};
can_rx_t rxcan_1 = {0};
can_tx_t txcan_2 = {0};
can_rx_t rxcan_2 = {0};

can_rx_t rxcan_3_standard = {0};


void fdcan_setting_init(FDCAN_HandleTypeDef *hfdcan, uint32_t offset)
{
	hfdcan->Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan->Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan->Init.AutoRetransmission = DISABLE;
	hfdcan->Init.TransmitPause = DISABLE;
	hfdcan->Init.ProtocolException = DISABLE;
	hfdcan->Init.NominalPrescaler = 12;
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


void can_group_tx_update(can_tx_t *tx, uint32_t id, uint32_t length, uint32_t STD_EXT, uint32_t RTR)
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
	rx->filter_config.FilterType = FDCAN_FILTER_MASK;

	rx->filter_config.FilterID1 = id;
	rx->filter_config.FilterID2 = mask_setting;
	HAL_FDCAN_ConfigFilter(rx->can_channel, &rx->filter_config);
}


void can_send_data(can_tx_t *tx)
{
	HAL_FDCAN_AddMessageToTxFifoQ(tx->can_channel, &tx->header, tx->data);
}

void can_init(void)
{
	fdcan_setting_init(COMMUNICATION_CAN_1, 0);
	fdcan_setting_init(COMMUNICATION_CAN_2, 512);
	fdcan_setting_init(COMMUNICATION_CAN_3, 1024);

	// txcan_1.can_channel = COMMUNICATION_CAN_1;
	// rxcan_1.can_channel = COMMUNICATION_CAN_1;
	// txcan_2.can_channel = COMMUNICATION_CAN_2;
	// rxcan_2.can_channel = COMMUNICATION_CAN_2;

	rxcan_3_standard.can_channel = COMMUNICATION_CAN_3;


	// HAL_FDCAN_Start(COMMUNICATION_CAN_1);
	// HAL_FDCAN_Start(COMMUNICATION_CAN_2);
	HAL_FDCAN_Start(COMMUNICATION_CAN_3);

	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
	// __HAL_FDCAN_ENABLE_IT(COMMUNICATION_CAN, FDCAN_IT_RX_FIFO1_NEW_MESSAGE);
	// HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	// HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	// __HAL_CAN_ENABLE_IT(COMMUNICATION_CAN, CAN_IT_TX_MAILBOX_EMPTY);
}

