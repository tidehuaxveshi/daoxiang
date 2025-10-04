#ifndef CAN_COM_H_
#define CAN_COM_H_
#include "fdcan.h"
#include "cong.h"
#include "stdint.h"
#include "stdio.h"
#define MASK_ALL 0x1FFFFFFF
#define RM_GROUP_1 0x200
#define RM_GROUP_2 0x1FF
#define RM_BASE_ID 0x200
#define RM_DATA_SIZE_TRANSMIT 8
#define RM_DATA_SIZE_RECEIVE 8

typedef struct can_tx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_TxHeaderTypeDef header_group1;
    FDCAN_TxHeaderTypeDef header_group2;
    uint8_t data_group1[8];
    uint8_t data_group2[8];
}can_tx_t;
typedef struct can_rx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_FilterTypeDef filter_config;
    FDCAN_RxHeaderTypeDef rx_header;
    uint8_t data[8];
}can_rx_t;
void can_send_data_two(can_tx_t *tx);
extern can_tx_t txcan_1;
extern can_rx_t rxcan_1;
extern can_tx_t txcan_2;
extern can_rx_t rxcan_2;
extern can_tx_t txcan_test;
extern can_rx_t rxcan_test;
void can_init(void);
#endif /* CAN_COM_H_ */