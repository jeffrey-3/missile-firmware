#include "indicator.h"

void indicator_init(indicator_t *indicator, gpio_t *led) {
    indicator->led = led;
    indicator->led_timer = 0;
    indicator->led_on = false;
    gpio_init(led);
}

void indicator_update_slow(indicator_t *indicator) {
    if (timer_expired(&indicator->led_timer, 1000)) {
        gpio_write(indicator->led, indicator->led_on);
        indicator->led_on = !indicator->led_on;
    }
}

void indicator_update_fast(indicator_t *indicator) {
    if (timer_expired(&indicator->led_timer, 200)) {
        gpio_write(indicator->led, indicator->led_on);
        indicator->led_on = !indicator->led_on;
    }
}

void indicator_update_solid(indicator_t *indicator) {
    gpio_write(indicator->led, false);
}
