#ifndef CAN_COM_H_
#define CAN_COM_H_
#include "fdcan.h"
#include "cong.h"
#include "stdint.h"


typedef struct can_tx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_TxHeaderTypeDef header;
    uint8_t data[8];
    uint8_t mail_status;
}can_tx_t;
typedef struct can_rx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_RxHeaderTypeDef cong_header;
    FDCAN_RxHeaderTypeDef rx_header;
    uint32_t mask_setting;
    uint32_t fifo_setting;
    uint32_t filter_bank;
    uint8_t data[8];
}can_rx_t;
void CAN_Send_Data(can_tx_t *tx);
extern can_tx_t txcan;
extern can_rx_t rxcan;
#endif /* CAN_COM_H_ */