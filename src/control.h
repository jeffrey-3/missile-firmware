#ifndef CONTROL_H
#define CONTROL_H

#include "hal/timer.h"

typedef struct {
    timer_t *servo_y;
    timer_t *servo_z;
} control_t;

void control_init(control_t *control, timer_t *servo_y, timer_t *servo_z);

#endif // CONTROL_H
