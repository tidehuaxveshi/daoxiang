#ifndef J60_H
#define J60_H
#include "cong.h"
#include "can_com.h"
#include "stdbool.h"
#define J60_ID_BIAS 0x10
// mapping:from raw data to physical value
// inverse mapping:from physical value to raw data
#define angle_mapping(x) (uint32_t)((x + 40.0f) / 80.0f * 65535)
#define angle_mapping_inv(x) (((float)x * 80.0f / 1048575.0f) - 40.0f)

#define velocity_mapping(x) (uint32_t)((x + 40.0f) / 80.0f * 16383)
#define velocity_mapping_inv(x) (((float)x * 1000.0f / 1048575.0f) - 40.0f)

#define torque_mapping(x) (uint32_t)((x + 40.0f) / 80.0f * 65535)
#define torque_mapping_inv(x) (((float)x * 1000.0f / 65535.0f) - 40.0f)

#define temperature_mapping_inverse(x) (((float)x + 100.0f) * 100.0f / 65535)

#define Kp_mapping(x) (uint32_t)(x)
#define Kd_mapping(x) (uint32_t)((x) * 255.0f / 51.0f)

typedef struct j60
{
    uint8_t motor_id;

    // raw data
    uint32_t angle_target_raw;
    uint32_t angle_current_raw;

    uint32_t velocity_target_raw;
    uint32_t velocity_current_raw;

    uint32_t torque_target_raw;
    uint32_t torque_current_raw;

    uint32_t temperature_raw;

    uint16_t Kp_raw;
    uint16_t Kd_raw;
    // modified data
    float angle_target;
    float angle_current;

    float velocity_target;
    float velocity_current;

    float torque_target;
    float torque_current;

    float temperature_mosfet;
    float temperature_motor;

    float Kp;
    float Kd;

    // flag
    bool temputure_flag;

} j60_t;
typedef struct j60_group
{
    can_tx_t j60_tx;
    j60_t j60[16];
} j60_group_t;
extern j60_group_t j60_group;
void j60_init();
void j60_group_data_acquisition(j60_group_t *tar, can_rx_t *can_rx);
void j60_group_control_set(j60_group_t *tar);
void j60_control_set(j60_t *tar, can_tx_t *can_tx);
#endif // J60_H