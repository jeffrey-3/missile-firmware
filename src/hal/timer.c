#include "timer.h"

void timer_init(timer_t *timer) {
    // Configure GPIO
    gpio_init(&timer->gpio);

    // Enable peripheral clock
    if (timer->tim_reg == TIM1) {
        RCC->APBENR2 |= 1UL << 11;
    } else if (timer->tim_reg == TIM3) {
        RCC->APBENR1 |= 1UL << 1;
    }

    timer->tim_reg->PSC = 15; // Divide timer clock
    timer->tim_reg->ARR = 2000; // Timer count to value from 0
}
