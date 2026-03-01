#include "../../src/hal/pwm.h"
#include "../../src/pins.h"
#include "sim_interface.h"

void pwm_init(pwm_t *timer, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel, float frequency) {
    (void)timer;
    (void)tim_reg;
    (void)channel;
    (void)frequency;
    timer->gpio = gpio;
}

void pwm_set_pulse(pwm_t *pwm, uint16_t us) {
    if (pwm->gpio == &pins.tim1_ch4) {
        sim_transmit.servo_y = us;
    } else if (pwm->gpio == &pins.tim3_ch2) {
        sim_transmit.servo_z = us;
    }
}
