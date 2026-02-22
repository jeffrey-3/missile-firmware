#include "timer.h"

void timer_init(timer_t *timer, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel) {
    timer->tim_reg = tim_reg;
    timer->gpio = gpio;
    timer->channel = channel;

    // Configure GPIO
    gpio_init(timer->gpio);

    // Enable peripheral clock
    if (timer->tim_reg == TIM1) {
        RCC->APBENR2 |= 1UL << 11;
    } else if (timer->tim_reg == TIM3) {
        RCC->APBENR1 |= 1UL << 1;
    }

    timer->tim_reg->PSC = 15; // Divide timer clock
    timer->tim_reg->ARR = 2000; // Timer count to value from 0

    if (timer->channel == 4) {
        // PWM Mode 1 (set OC4M to 0110)
        timer->tim_reg->CCMR3 |= 3UL << 13;

        // Enable output on channel 4 (set CC4E to 1)
        timer->tim_reg->CCER |= 1UL << 12;
    } else if (timer->channel == 2) {
        timer->tim_reg->CCMR1 |= 3UL << 13;
        timer->tim_reg->CCER |= 1UL << 4;
    }

    // Enable the clock counter
    timer->tim_reg->CR1 |= 1UL;
}
