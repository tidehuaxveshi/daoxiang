#include "motion_planing.h"

// void cubic_poly_coefficients_cal(double *theta, double *theta_dot, double *theta_t, double *theta_dot_t, double t, double *coefficents)
// {
//     coefficents[0] = *theta;
//     coefficents[1] = *theta_dot;
//     coefficents[2] = (3 * (*theta_t - *theta) / (t * t)) - (2 * *theta_dot + *theta_dot_t) / t;
//     coefficents[3] = (-2 * (*theta_t - *theta) / (t * t * t)) + (*theta_dot + *theta_dot_t) / (t * t);
// }
// void cubic_polynomial_trajectory_cal(angle_planing_t *tar)
// {

//     for (int i = 0; i < 6; i++)
//     {
//         cubic_poly_coefficients_cal(tar->theta + i, tar->theta_dot + i, tar->theta_target + i, tar->theta_dot_target + i, tar->time_cost, tar->cubic_coefficients + 4 * i);
//     }
//     tar->state = TROJECT_EXECUTE;
//     tar->time_count = 0;
//     tar->time_count_target = (int)(tar->time_cost * (double)(MOTOR_CONTROL_FREQ));
// }
// void angle_cal_from_poly(double *a, double t, double *theta)
// {
//     *theta = a[0] + a[1] * t + a[2] * t * t + a[3] * t * t * t;
// }

// void angle_cal(angle_planing_t *tar)
// {
//     if (tar->state != TROJECT_EXECUTE)
//         return;
//     if (tar->time_count == tar->time_count_target)
//     {
//         tar->state = TROJECT_PLANNING;
//         tar->time_count = 0;
//         return;
//     }
//     for (int i = 0; i < 6; i++)
//     {
//         angle_cal_from_poly(tar->cubic_coefficients + 4 * i, tar->time_count / (double)MOTOR_CONTROL_FREQ, tar->theta + i);
//     }
//     tar->time_count++;
// }
