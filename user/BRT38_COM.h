#ifndef BRT38_COM_H_
#define BRT38_COM_H_

#include "cong.h"
#include "can_com.h"

#define READ_CMD 0x01
#define RESOLUTION 4096
#define CIRCLE_MAX 32
void BRT38_read_cmd(uint8_t id);
typedef struct BRT38_COM{
    can_tx_t BRT_38_tx;
    uint32_t raw_angle;

    float angle;
}BRT38_COM_t;

extern BRT38_COM_t BRT38_COM;
#endif 