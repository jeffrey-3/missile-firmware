#ifndef TIMER_H
#define TIMER_H

#include "gpio.h"
#include "registers.h"

typedef struct {
    struct tim_reg *tim_reg;
    gpio_t *gpio;
    uint8_t channel;
} timer_t;

void timer_init(timer_t *timer, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel);

#endif // TIMER_H
