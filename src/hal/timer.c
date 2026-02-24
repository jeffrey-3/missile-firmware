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
        timer->tim_reg->BDTR |= 1UL << 15; // Enable main output
    } else if (timer->tim_reg == TIM3) {
        RCC->APBENR1 |= 1UL << 1;
    }

    timer->tim_reg->PSC = FREQ / 1000000 - 1; // Divide timer clock to 1 MHz
    timer->tim_reg->ARR = 3000; // Timer count to value from 0

    if (timer->channel == 4) {
        timer->tim_reg->CCMR2 |= 3UL << 13; // PWM Mode 1 (OC4M = 0110)
        timer->tim_reg->CCER |= 1UL << 12; // Enable output on CH4 (CC4E = 1)
    } else if (timer->channel == 2) {
        timer->tim_reg->CCMR1 |= 3UL << 13;
        timer->tim_reg->CCER |= 1UL << 4;
    }

    // Enable the clock counter
    timer->tim_reg->CR1 |= 1UL;
}

// Duty is a float from 0 to 1
void timer_set_duty(timer_t *timer, float duty) {
    if (timer->channel == 2) {
        timer->tim_reg->CCR2 = (uint32_t)(duty *
            (float)timer->tim_reg->ARR + 1);
    } else if (timer->channel == 4) {
        timer->tim_reg->CCR4 = (uint32_t)(duty *
            (float)timer->tim_reg->ARR + 1);
    }
}
