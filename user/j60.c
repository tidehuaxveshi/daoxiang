#include "j60.h"
j60_group_t j60_group = {0};
/**
 * @brief physical values to the corresponding raw values
 * @param tar pointer to the j60 structure
 * @return null
 * @note null
 */
void j60_data_mapping(j60_t *tar)
{
    tar->angle_target_raw = angle_mapping(tar->angle_target);
    tar->velocity_target_raw = velocity_mapping(tar->velocity_target);
    tar->torque_target_raw = torque_mapping(tar->torque_target);
    tar->Kp_raw = Kp_mapping(tar->Kp);
    tar->Kd_raw = Kd_mapping(tar->Kd);
}
void j60_group_data_mapping(j60_group_t *tar, uint8_t *data)
{
    for (int i = 0; i < 16; i++)
    {
        j60_data_mapping(&tar->j60[i]);
    }
}
/**
 * @brief raw values to the corresponding physical values
 * @param tar pointer to the j60 structure
 * @return null
 * @note null
 */
void j60_mapping_inverse(j60_t *tar)
{
    tar->angle_current = angle_mapping_inv(tar->angle_current_raw);
    tar->velocity_current = velocity_mapping_inv(tar->velocity_current_raw);
    tar->torque_current = torque_mapping_inv(tar->torque_current_raw);
    if (tar->temputure_flag)
    {
        tar->temperature_motor = temperature_mapping_inverse(tar->temperature_raw);
    }
    else
    {
        tar->temperature_mosfet = temperature_mapping_inverse(tar->temperature_raw);
    }
}
void j60_group_mapping_inverse(j60_group_t *tar)
{
    for (int i = 0; i < 16; i++)
    {
        j60_mapping_inverse(&tar->j60[i]);
    }
}
/**
 * @brief extract the data from the can message and store it in the j60 structure
 * @param tar pointer to the j60 structure
 * @param data pointer to the can message data
 * @return null
 * @note null
 */
void j60_data_acquisition(j60_t *tar, uint8_t *data)
{

    tar->angle_current_raw = (data[2] & 0xF) << 16 | data[1] << 8 | data[0];
    tar->velocity_current_raw = data[4] << 16 | data[3] << 8 | (data[2] & 0xF0) >> 4;
    tar->torque_current_raw = data[6] << 8 | data[5];
    tar->temperature_raw = data[7] >> 1;
    tar->temputure_flag = data[7] & 0x01;
    j60_mapping_inverse(tar);
}
/**
 * @brief extract the data from the can message and store it in the j60 structure
 * @param tar pointer to the j60 structure
 * @param can_rx pointer to the can message data
 * @return null
 * @note null
 */
void j60_group_data_acquisition(j60_group_t *tar, can_rx_t *can_rx)
{
    if (can_rx->rx_header.Identifier >= 0 && can_rx->rx_header.Identifier < 16)
    {
        j60_data_acquisition(&tar->j60[can_rx->rx_header.Identifier - J60_ID_BIAS], can_rx->data);
    }
}

void j60_control_set(j60_t *tar, can_tx_t *can_tx)
{
    can_tx->header.Identifier = 0x4 << 5 | tar->motor_id;
    can_tx->data[0] = (uint8_t)(tar->angle_target_raw & 0xFF);
    can_tx->data[1] = (uint8_t)(tar->angle_target_raw & 0xFF00) >> 8;
    can_tx->data[2] = (uint8_t)(tar->velocity_target_raw & 0xFF);
    can_tx->data[3] = (uint8_t)(tar->velocity_target_raw & 0x3FC0) >> 8 | (uint8_t)(tar->Kp_raw & 0x3);
    can_tx->data[4] = (uint8_t)(tar->Kp_raw & 0x3FC) >> 2;
    can_tx->data[5] = (uint8_t)(tar->Kd_raw & 0xFF);
    can_tx->data[6] = (uint8_t)(tar->torque_target_raw & 0xFF);
    can_tx->data[7] = (uint8_t)(tar->torque_target_raw & 0xFFF0) >> 8 ;
    can_send_data(can_tx);
}
void j60_init()
{
    j60_group.j60_tx.can_channel = COMMUNICATION_CAN_3;
    j60_group.j60[1].motor_id = 1;
}
void j60_group_control_set(j60_group_t *tar)
{
    for (int i = 0; i < 16; i++)
    {
        j60_control_set(&tar->j60[i], &tar->j60_tx);
    }
}