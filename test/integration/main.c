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

    gpio_t tim1_ch4 = {
        .gpio_reg = GPIOA,
        .pin = 11,
        .af = 2,
        .mode = GPIO_MODE_AF
    };

    timer_t servo;
    timer_init(&servo, TIM1, &tim1_ch4, 4);

    bool led_on = false;
    uint32_t led_timer = 0;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            gpio_write(&led, led_on);
            led_on = !led_on;
        }

        timer_set_duty(&servo, 0.5);
        delay(1000);
    }

    return 0;
}
