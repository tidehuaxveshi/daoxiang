#include "rm_motor.h"
rm_motor_group_t wheel = {0};
pid wheel_pid[8] = {0};
void current_adjust(rm_motor_group_t *tar, int index, float current)
{
    tar->current_set_float[index] = current;
}
void current_adjust_all(rm_motor_group_t *tar, float current)
{
    for (int i = 0; i < 8; i++)
    {
        tar->current_set_float[i] = current;
    }
}
/*
tx1 should be 0x200 while tx2 should be 0x1ff
*/
void current_set(rm_motor_group_t *tar, can_tx_t *tx)
{
    send_mapping(tar);
    for (int i = 0; i < 4; i++)
    {
        tx->data_group1[i * 2] = (tar->current_set[i] >> 8);
        tx->data_group1[i * 2 + 1] = tar->current_set[i];
        tx->data_group2[i * 2] = (tar->current_set[i + 4] >> 8);
        tx->data_group2[i * 2 + 1] = tar->current_set[i + 4];
    }
    can_send_data_two(tx);
}
void data_extract(rm_motor_group_t *tar, can_rx_t *rx)
{
    int16_t position;
    int16_t velocity;
    int16_t current;
    uint8_t temp;
    position = (int16_t)(((uint16_t)(rx->data[0]) << 8) | rx->data[1]);
    velocity = (int16_t)(((uint16_t)(rx->data[2]) << 8) | rx->data[3]);
    current = (int16_t)(((uint16_t)(rx->data[4]) << 8) | rx->data[5]);
    temp = rx->data[6];

    int id_rx;
    id_rx = rx->rx_header.Identifier - RM_BASE_ID;
    if (id_rx < 9 && id_rx > 0)
    {
        tar->current_now[id_rx-1] = current;
        tar->position[id_rx-1] = position;
        tar->velocity[id_rx-1] = velocity;
        tar->temp[id_rx-1] = temp;
    }
    else
    {
        return;
    }
}
void send_mapping(rm_motor_group_t *tar)
{
    for (int i = 0; i < 8; i++)
    {
        tar->current_set[i] = (int16_t)(tar->current_set_float[i] * MAPPING_FACTOR);
    }
}
void pid_cal(pid *cal)
{
    cal->error_last = cal->error;
    cal->error = cal->target - cal->measure;
    cal->integral = cal->integral + cal->error;
    if (cal->integral > cal->integral_limit)
    {
        cal->integral = cal->integral_limit;
    }
    else if (cal->integral < -cal->integral_limit)
    {
        cal->integral = -cal->integral_limit;
    }

    cal->output = cal->Kp * cal->error + cal->Ki * cal->integral + cal->Kd * (cal->error - cal->error_last);
    if (cal->output > cal->output_limit)
    {
        cal->output = cal->output_limit;
    }
    else if (cal->output < -cal->output_limit)
    {
        cal->output = -cal->output_limit;
    }
}
void pid_init(pid *pid_init, float p, float i, float d, float integral_limit, float output_limit)
{
    pid_init->Kp = p;
    pid_init->Ki = i;
    pid_init->Kd = d;
    pid_init->integral = 0;
    pid_init->integral_limit = integral_limit;
    pid_init->output_limit = output_limit;
}
