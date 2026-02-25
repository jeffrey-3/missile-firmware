#ifndef PWM_H
#define PWM_H

#include "clock.h"
#include "gpio.h"
#include "registers.h"

typedef struct {
    struct tim_reg *tim_reg;
    gpio_t *gpio;
    uint8_t channel;
    float frequency;
} pwm_t;

void pwm_init(pwm_t *timer, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel, float frequency);
void pwm_set_pulse(pwm_t *pwm, uint16_t us);

#endif // PWM_H
