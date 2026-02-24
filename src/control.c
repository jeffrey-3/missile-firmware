#include "control.h"

void control_init(control_t *control, pwm_t *servo_y, pwm_t *servo_z) {
    control->servo_y = servo_y;
    control->servo_z = servo_z;
}

void control_update(control_t *control, ins_t *ins) {
    ins = ins;

    pwm_set_pulse(control->servo_y, 1520);
    pwm_set_pulse(control->servo_z, 1520);
}
