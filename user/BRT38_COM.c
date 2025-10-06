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