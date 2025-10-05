#ifndef RM_MOTOR_H_
#define RM_MOTOR_H_

#include "can_com.h"

#define MAPPING_FACTOR (float)(16384/20)
#define PI 3.14159265358979323846

#define LIMITI(current, li) (current > ((SPWM_RESOLUTION >> 1) + li) ? ((SPWM_RESOLUTION >> 1) + li) : (current < (SPWM_RESOLUTION >> 1) - li) ? ((SPWM_RESOLUTION >> 1) - li) \: current)
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
    float current_set_float[8];
    int16_t current_set[8];
    int16_t position[8];
    int16_t velocity[8];
    int16_t current_now[8];
    uint8_t temp[8];
}rm_motor_group_t;
extern rm_motor_group_t wheel;  
extern pid wheel_pid[8];
void data_extract(rm_motor_group_t *tar, can_rx_t *rx);
void current_set(rm_motor_group_t *tar, can_tx_t *tx);
void send_mapping(rm_motor_group_t *tar);
void current_adjust(rm_motor_group_t *tar, int index, float current);
void current_adjust_all(rm_motor_group_t *tar, float current);
#endif /* RM_MOTOR_H_ */