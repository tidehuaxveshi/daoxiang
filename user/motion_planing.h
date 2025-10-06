#ifndef TROJECRTION_H_
#define TROJECRTION_H_
#include "rm_motor.h"

#include <math.h>

typedef struct arm
{

    double theta[6];
    double theta_dot[6];
    double theta_target[6];
    double theta_dot_target[6];
    double time_cost;
    int time_count, time_count_target;

    double cubic_coefficients[4 * 6];

    int state;

} angle_planing_t;

enum
{
    TROJECT_PLANNING = 0,
    TROJECT_EXECUTE = 1,
};

void angle_cal(angle_planing_t *tar);
void angle_cal_from_poly(double *a, double t, double *theta);
void cubic_polynomial_trajectory_cal(angle_planing_t *tar);
#endif /* TROJECRTION_H_ */