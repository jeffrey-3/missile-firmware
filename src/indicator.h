#ifndef INDICATOR_H
#define INDICATOR_H

#include "hal/gpio.h"
#include "hal/clock.h"

typedef struct {
    gpio_t *led;
    uint32_t led_timer;
    bool led_on;
} indicator_t;

void indicator_init(indicator_t *indicator, gpio_t *led);
void indicator_update_slow(indicator_t *indicator);
void indicator_update_fast(indicator_t *indicator);
void indicator_update_solid(indicator_t *indicator);

#endif // INDICATOR_H
