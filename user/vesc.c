#include "vesc.h"
can_tx_t vesc_can_tx;
void vesc_init()
{
    vesc_can_tx.can_channel=COMMUNICATION_CAN_3;
}
// Implementation for sending extended ID CAN-frames
void mapping_and_send(uint32_t id, uint32_t cmd_id, float data, float scale)
{
    id = id | (cmd_id << 8);
    data_mapping(vesc_can_tx.data, (int32_t)(data * scale));
    can_group_tx_update(&vesc_can_tx, id, 4, FDCAN_EXTENDED_ID, FDCAN_DATA_FRAME);
    can_send_data(&vesc_can_tx);
}
void data_mapping(uint8_t *data, int32_t value)
{
    data[0] = (value >> 24) & 0xFF;
    data[1] = (value >> 16) & 0xFF;
    data[2] = (value >> 8) & 0xFF;
    data[3] = value & 0xFF;
}

void comm_can_set_duty(uint32_t controller_id, float duty)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_DUTY, duty, DUTY_SCALE);
}

void comm_can_set_current(uint32_t controller_id, float current)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT, current, CURRENT_SCALE);
}

void comm_can_set_current_brake(uint32_t controller_id, float current)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT_BRAKE, current, CURRENTT_BRAKE_SCALE);
}

void comm_can_set_rpm(uint32_t controller_id, float rpm)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_RPM, rpm, RPM_SCALE);
}

void comm_can_set_pos(uint32_t controller_id, float pos)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_POS, pos, POS_SCALE);
}

void comm_can_set_current_rel(uint32_t controller_id, float current_rel)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT_REL, current_rel, CURRENT_REL_SCALE);
}
void comm_can_set_current_brake_rel(uint32_t controller_id, float current_rel)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT_BRAKE_REL, current_rel, CURRENT_BRAKE_REL_SCALE);
}
void comm_can_set_current_handbrake(uint32_t controller_id, float current)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT_HANDBRAKE, current, CURRENT_HANDBRAKE_SCALE);
}
void comm_can_set_current_handbrake_rel(uint32_t controller_id, float current_rel)
{
    mapping_and_send(controller_id, CAN_PACKET_SET_CURRENT_HANDBRAKE_REL, current_rel, CURRENT_HANDBRAKE_REL_SCALE);
}

void comm_can_set_current_off_delay(uint8_t controller_id, float current, float off_delay)
{

    uint8_t buffer[6];
    buffer_append_int32(buffer, (int32_t)(current * 1000.0));
    buffer_append_float16(buffer, off_delay, 1e3);
    can_transmit_eid(controller_id |
                         ((uint32_t)CAN_PACKET_SET_CURRENT << 8),
                     buffer);
}

/**
 * Same as above, but also sets the off delay. Note that this command uses 6 bytes now. The off delay is useful to set to keep the current controller running for a while even after setting currents below the minimum current.
 */
void comm_can_set_current_rel_off_delay(uint8_t controller_id, float current_rel, float off_delay)
{

    uint8_t buffer[6];
    buffer_append_float32(buffer, current_rel, 1e5);
    buffer_append_float16(buffer, off_delay, 1e3);
    can_transmit_eid(controller_id |
                         ((uint32_t)CAN_PACKET_SET_CURRENT_REL << 8),
                     buffer);
}

