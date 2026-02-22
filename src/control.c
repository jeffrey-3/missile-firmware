#include "control.h"

void control_init(control_t *control, timer_t *servo_y, timer_t *servo_z) {
    control->servo_y = servo_y;
    control->servo_z = servo_z;
}

void control_update(control_t *control, ins_t *ins) {
    control =control;
    ins = ins;
    return;
}
