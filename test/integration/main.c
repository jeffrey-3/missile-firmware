#include "../../src/hal/timer.h"
#include "../../src/hal/clock.h"

int main(void) {
    systick_init();

    gpio_t led = {
        .gpio_reg = GPIOB,
        .pin = 9,
        .af = 0,
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_init(&led);

    gpio_t tim3_ch2 = {
        .gpio_reg = GPIOA,
        .pin = 7,
        .af = 1,
        .mode = GPIO_MODE_AF
    };
    gpio_t tim1_ch4 = {
        .gpio_reg = GPIOA,
        .pin = 11,
        .af = 2,
        .mode = GPIO_MODE_AF
    };

    timer_t servo_y;
    timer_t servo_z;
    timer_init(&servo_y, TIM3, &tim3_ch2, 2);
    timer_init(&servo_z, TIM1, &tim1_ch4, 4);

    bool led_on = false;
    uint32_t led_timer = 0;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            gpio_write(&led, led_on);
            led_on = !led_on;

            if (led_on) {
                timer_set_duty(&servo_y, 0.7f);
                timer_set_duty(&servo_z, 0.5f);
            } else {
                timer_set_duty(&servo_y, 0.3f);
                timer_set_duty(&servo_z, 0.6f);
            }
        }
    }

    return 0;
}
