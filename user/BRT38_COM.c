#include "BRT38_COM.h"
can_tx_t BRT_38_tx = {0};
void BRT38_read_cmd(uint8_t id)
{
    can_group_tx_update(&BRT_38_tx, id, 4, FDCAN_STANDARD_ID, FDCAN_DATA_FRAME);
    BRT_38_tx.data_group[0] = READ_CMD;
    BRT_38_tx.data_group[1] = id;
    BRT_38_tx.data_group[2] = 0x01;
    BRT_38_tx.data_group[3] = 0x00;
    can_send_data(&BRT_38_tx);
}
void BRT38_angle_acquisition(BRT38_COM_t* brt38_com, can_rx_t *rx)
{
    if(rx->rx_header.Identifier == READ_CMD)
    {
        brt38_com->raw_angle = (rx->data[0]) | (rx->data[1] << 8) | (rx->data[2] << 16) | (rx->data[3]<<24);
    }
    brt38_com->angle = (float)brt38_com->raw_angle*360.0f / RESOLUTION;
}