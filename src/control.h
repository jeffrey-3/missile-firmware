#ifndef CONTROL_H
#define CONTROL_H

#include "hal/pwm.h"
#include "ins.h"

typedef struct {
    pwm_t *servo_y;
    pwm_t *servo_z;
    float roll;
    float pitch;
    float yaw;
    uint16_t max_pulse;
    uint16_t min_pulse;
    float p_gain;
} control_t;

void control_init(control_t *control, pwm_t *servo_y, pwm_t *servo_z);
void control_update(control_t *control, ins_t *ins);

#endif // CONTROL_H
