#ifndef RM_MOTOR_H_
#define RM_MOTOR_H_

#include "can_com.h"

#define CURRENT_MAPPING_FACTOR (float)(16384 / 20)
#define POSITION_MAPPING_FACTOR (float)(8191 / 360)
#define PI 3.14159265358979323846
#define ENCODER_RESOLUTION 8192
#define LIMITI(current, li) (current > ((SPWM_RESOLUTION >> 1) + li) ? ((SPWM_RESOLUTION >> 1) + li) : (current < (SPWM_RESOLUTION >> 1) - li) ? ((SPWM_RESOLUTION >> 1) - li) \ \
                                                                                                                                               : current)
#define ANGLE_ELE_LIMITI(angle, lower_bound, upper_bound) ((angle > upper_bound) ? (angle - upper_bound) : ((angle < lower_bound) ? (angle + upper_bound) : (angle)))

typedef struct pid
{
    float target;
    float measure;
    float error;
    float error_last;
    float Kp, Ki, Kd;
    float output;
    float integral;
    float integral_limit;
    float output_limit;
} pid;
typedef struct rm_motor_group
{
    // not normalized data
    int16_t current_set[8];
    int16_t current_now[8];

    int16_t velocity[8];
    int16_t velocity_target[8];

    int16_t position[8];
    int16_t position_last[8];

    int16_t cycle[8];
    int32_t position_absolute[8];

    uint8_t temp[8];
    // normalized data
    float current_set_modified[8];
    float current_modified[8];

    float position_modified[8];

    float position_absolute_modified[8];

    float position_target_modified[8];

    // pid
    pid velocity_pid[8];
    pid position_pid[8];

    pid velocity_pid_inner[8];
    pid position_pid_outter[8];

} rm_motor_group_t;
extern rm_motor_group_t wheel;
extern pid wheel_pid[8];
void pid_init(pid *pid_init, float p, float i, float d, float integral_limit, float output_limit);
void data_extract(rm_motor_group_t *tar, can_rx_t *rx);
void current_set(rm_motor_group_t *tar, can_tx_t *tx);
void send_mapping(rm_motor_group_t *tar);
void receive_mapping(rm_motor_group_t *tar);
void current_adjust(rm_motor_group_t *tar, int index, float current);
void current_adjust_all(rm_motor_group_t *tar, float current);
void single_velocity_loop_cal(rm_motor_group_t *group, uint8_t index);
void all_velocity_loop_cal(rm_motor_group_t *group);
void single_position_loop_cal(rm_motor_group_t *group, uint8_t index);
void all_position_loop_cal(rm_motor_group_t *group);
void absolute_angle_cal(rm_motor_group_t *tar);
void single_serial_loop_cal(rm_motor_group_t *group, uint8_t index);
void all_serial_position_loop_cal(rm_motor_group_t *group);
#endif /* RM_MOTOR_H_ */