#include "../../src/hal/clock.h"
#include "../../src/pins.h"

void test_led_blink(void) {
    gpio_init(&pins.led);

    bool led_on = false;

    for (;;) {
        gpio_write(&pins.led, led_on);
        led_on = !led_on;
        delay(200);
    }
}
