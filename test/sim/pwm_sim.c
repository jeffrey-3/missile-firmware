#include "../../src/hal/pwm.h"

void pwm_init(pwm_t *timer, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel, float frequency) {
    (void)timer;
    (void)tim_reg;
    (void)gpio;
    (void)channel;
    (void)frequency;
}

void pwm_set_pulse(pwm_t *pwm, uint16_t us) {
    (void)pwm;
    (void)us;
}
