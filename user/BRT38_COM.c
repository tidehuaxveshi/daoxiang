#include "BRT38_COM.h"
BRT38_COM_t BRT38 = {0};
void BRT38_read_cmd(uint8_t id)
{
    can_group_tx_update(&BRT38.BRT38_can_tx, 2, 4, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);
    BRT38.BRT38_can_tx.data[0] = READ_CMD;
    BRT38.BRT38_can_tx.data[1] = id;
    BRT38.BRT38_can_tx.data[2] = 0x01;
    BRT38.BRT38_can_tx.data[3] = 0x00;
    can_send_data(&BRT38.BRT38_can_tx);
}
void BRT38_angle_acquisition(BRT38_COM_t* brt38_com, can_rx_t *rx)
{
    if(rx->rx_header.Identifier == READ_CMD)
    {
        brt38_com->raw_angle = (rx->data[3]) | (rx->data[4] << 8) | (rx->data[5] << 16) | (rx->data[6]<<24);
    }
    brt38_com->angle = (float)brt38_com->raw_angle*360.0f / RESOLUTION;
}
void BRT38_init()
{
    BRT38.BRT38_can_tx.can_channel=COMMUNICATION_CAN_3;
}