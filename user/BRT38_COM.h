#ifndef BRT38_COM_H_
#define BRT38_COM_H_

#include "cong.h"
#include "can_com.h"

#define READ_CMD 0x01
#define RESOLUTION 4096
#define CIRCLE_MAX 32
void BRT38_read_cmd(uint8_t id);
typedef struct BRT38_COM{
    can_tx_t BRT38_can_tx;
    uint32_t raw_angle;

    float angle;
}BRT38_COM_t;
void BRT38_init();
void BRT38_angle_acquisition(BRT38_COM_t* brt38_com, can_rx_t *rx);
extern BRT38_COM_t BRT38;
#endif 