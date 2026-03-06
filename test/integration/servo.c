#include "../../src/hal/pwm.h"
#include "../../src/pins.h"

void test_servo(void) {
    gpio_init(&pins.led);

    pwm_t servo_y;
    pwm_t servo_z;
    pwm_init(&servo_y, TIM3, &pins.tim3_ch2, 2, 333.0f);
    pwm_init(&servo_z, TIM1, &pins.tim1_ch4, 4, 333.0f);

    bool led_on = false;

    for (;;) {
        gpio_write(&pins.led, led_on);
        led_on = !led_on;

        if (led_on) {
            pwm_set_pulse(&servo_y, 2100);
            delay(200);
            pwm_set_pulse(&servo_y, 900);
            delay(200);
            pwm_set_pulse(&servo_y, 1500);
            delay(200);
        } else {
            pwm_set_pulse(&servo_z, 2100);
            delay(200);
            pwm_set_pulse(&servo_z, 900);
            delay(200);
            pwm_set_pulse(&servo_z, 1500);
            delay(200);
        }
    }
}
