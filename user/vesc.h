#ifndef VESC_H
#define VESC_H

#include <stdint.h>
#include "cong.h"
#include "can_com.h"

#define DUTY_SCALE 100000.0f
#define CURRENT_SCALE 1000.0f
#define CURRENTT_BRAKE_SCALE 1000.0f
#define RPM_SCALE 1.0f
#define POS_SCALE 1000000.0f
#define CURRENT_REL_SCALE 100000.0f
#define CURRENT_BRAKE_REL_SCALE 100000.0f
#define CURRENT_HANDBRAKE_SCALE 1000.0f
#define CURRENT_HANDBRAKE_REL_SCALE 100000.0f

typedef enum
{
    CAN_PACKET_SET_DUTY = 0,
    CAN_PACKET_SET_CURRENT,
    CAN_PACKET_SET_CURRENT_BRAKE,
    CAN_PACKET_SET_RPM,
    CAN_PACKET_SET_POS,
    CAN_PACKET_SET_CURRENT_REL = 10,
    CAN_PACKET_SET_CURRENT_BRAKE_REL,
    CAN_PACKET_SET_CURRENT_HANDBRAKE,
    CAN_PACKET_SET_CURRENT_HANDBRAKE_REL,
    CAN_PACKET_MAKE_ENUM_32_BITS = 0xFFFFFFFF,
} CAN_PACKET_ID;
typedef struct vesc
{
    uint32_t controller_id;
}vesc_controller_t;
    
void comm_can_set_current(uint32_t controller_id, float current);
void comm_can_set_rpm(uint32_t controller_id, float rpm);
void comm_can_set_pos(uint32_t controller_id, float pos);
void vesc_init();
#endif // VESC_H
