#include "rm_motor.h"
rm_motor_group_t wheel = {0};
/*
tx1 should be 0x200 while tx2 should be 0x1ff
*/
void current_set(rm_motor_group_t *tar, can_tx_t *tx)
{
    send_mapping(tar,tx);
    for (int i = 0; i < 4; i++)
    {
        tx->data_group1[i * 2] = (tar->current_set[i] >> 8);
        tx->data_group1[i * 2 + 1] = tar->current_set[i];
        tx->data_group2[i * 2] = (tar->current_set[i + 4] >> 8);
        tx->data_group2[i * 2 + 1] = tar->current_set[i + 4];
    }
    CAN_Send_Data(tx);
}
void data_extract(rm_motor_group_t *tar, can_rx_t *rx)
{
    int16_t position;
    int16_t velocity;
    int16_t current;
    uint8_t temp;
    position = (int16_t)(((uint16_t)rx->data[0] << 8) | rx->data[1]);
    velocity = (int16_t)(((uint16_t)rx->data[2] << 8) | rx->data[3]);
    current = (int16_t)(((uint16_t)rx->data[4] << 8) | rx->data[5]);
    temp = rx->data[6];

    int i = rx->rx_header.Identifier - RM_BASE_ID;
    if (i >= 8)
    {
        return;
    }
    tar->current_now[i] = current;
    tar->position[i] = position;
    tar->velocity[i] = velocity;
    tar->temp[i] = temp
}
void send_mapping(rm_motor_group_t *tar, can_tx_t *tx)
{
    for(int i=0;i<8;i++)
    {
        tar->current_set[i] = (int16_t)(tar->current_set_float[i]*MAPPING_FACTOR);
    }
}