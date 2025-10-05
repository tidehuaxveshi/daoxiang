#include "rm_motor.h"
rm_motor_group_t wheel = {0};
void current_adjust(rm_motor_group_t *tar, int index, float current)
{
    tar->current_set_modified[index] = current;
}
void current_adjust_all(rm_motor_group_t *tar, float current)
{
    for (int i = 0; i < 8; i++)
    {
        tar->current_set_modified[i] = current;
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
        tar->current_now[id_rx - 1] = current;
        tar->position_last[id_rx - 1] = tar->position[id_rx - 1];
        tar->position[id_rx - 1] = position;
        tar->velocity[id_rx - 1] = velocity;
        tar->temp[id_rx - 1] = temp;

        absolute_angle_cal(tar);
        receive_mapping(tar);
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
        tar->current_set[i] = (int16_t)(tar->current_set_modified[i] * CURRENT_MAPPING_FACTOR);
    }
}
void receive_mapping(rm_motor_group_t *tar)
{
    for (int i = 0; i < 8; i++)
    {
        tar->current_modified[i] = ((float)tar->current_now[i] / CURRENT_MAPPING_FACTOR);
        tar->position_modified[i] = ((float)tar->position[i] / POSITION_MAPPING_FACTOR);
        tar->position_absolute_modified[i]=tar->position_modified[i]+tar->cycle[i]*360.0f;
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
// void pid_group(rm_motor_group_t *tar)
// {
//     tar->wheel_velocity_pid
// }
/*
input:group of pid
*/
void single_velocity_loop_cal(rm_motor_group_t *group, uint8_t index)
{
    (group->velocity_pid + index)->measure = group->velocity[index];
    (group->velocity_pid + index)->target = group->velocity_target[index];
    pid_cal(group->velocity_pid + index);
    group->current_set_modified[index] = (group->velocity_pid + index)->output;
}
void all_velocity_loop_cal(rm_motor_group_t *group)
{
    for (int i = 0; i < 8; i++)
    {
        single_velocity_loop_cal(group, i);
    }
}
void single_position_loop_cal(rm_motor_group_t *group, uint8_t index)
{
    (group->position_pid + index)->measure = group->position_absolute_modified[index];
    (group->position_pid + index)->target = group->position_target_modified[index];
    pid_cal(group->position_pid + index);
    group->current_set_modified[index] = (group->position_pid + index)->output;
}
void all_position_loop_cal(rm_motor_group_t *group)
{
    for (int i = 0; i < 8; i++)
    {
        single_position_loop_cal(group, i);
    }
}
void absolute_angle_cal(rm_motor_group_t *tar)
{
    for (int i = 0; i < 8; i++)
    {
        if ((tar->position[i] - tar->position_last[i]) >> 15 == 1 && tar->velocity[i] == 0)
        {
            tar->cycle[i]--;
        }
        else if ((tar->position[i] - tar->position_last[i]) >> 15 == 0 && tar->velocity[i] == 1)
        {
            tar->cycle[i]++;
        }
        tar->position_absolute[i] = tar->cycle[i] * ENCODER_RESOLUTION + tar->position[i];
    }
}
