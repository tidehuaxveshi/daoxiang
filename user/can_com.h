#ifndef CAN_COM_H_
#define CAN_COM_H_
#include "fdcan.h"
#include "cong.h"
#include "stdint.h"
#include "stdio.h"
#define MASK_ALL 0x1FFFFFFF

#define ENCODER_ADDR_1 0x01

#define RM_GROUP_1 0x200
#define RM_GROUP_2 0x1FF
#define RM_BASE_ID 0x200
#define RM_DATA_SIZE_TRANSMIT 8
#define RM_DATA_SIZE_RECEIVE 8

typedef struct can_tx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_TxHeaderTypeDef header;
    uint8_t data[8];
}can_tx_t;
typedef struct can_rx{
    FDCAN_HandleTypeDef *can_channel;
    FDCAN_FilterTypeDef filter_config;
    FDCAN_RxHeaderTypeDef rx_header;
    uint8_t data[8];
}can_rx_t;
extern can_rx_t rxcan_3_standard;
void can_group_tx_update(can_tx_t *tx, uint32_t id, uint32_t length, uint32_t STD_EXT, uint32_t RTR);
void can_send_data(can_tx_t *tx);
void can_fd_group_tx_update(can_tx_t *tx, uint32_t id, uint32_t length, uint32_t STD_EXT, uint32_t RTR);
extern can_tx_t txcan_1;
extern can_rx_t rxcan_1;
extern can_tx_t txcan_2;
extern can_rx_t rxcan_2;
extern can_tx_t txcan_3;
extern can_rx_t rxcan_3;
void can_init(void);
#endif /* CAN_COM_H_ */