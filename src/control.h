#ifndef CONTROL_H
#define CONTROL_H

#include "hal/timer.h"
#include "ins.h"

typedef struct {
    timer_t *servo_y;
    timer_t *servo_z;
} control_t;

void control_init(control_t *control, timer_t *servo_y, timer_t *servo_z);
void control_update(control_t *control, ins_t *ins);

#endif // CONTROL_H
