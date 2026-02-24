#include "pwm.h"

void pwm_init(pwm_t *pwm, struct tim_reg *tim_reg, gpio_t *gpio,
    uint8_t channel, float frequency) {
    pwm->tim_reg = tim_reg;
    pwm->gpio = gpio;
    pwm->channel = channel;
    pwm->frequency = frequency;

    // Configure GPIO
    gpio_init(pwm->gpio);

    // Enable peripheral clock
    if (pwm->tim_reg == TIM1) {
        RCC->APBENR2 |= 1UL << 11;
        pwm->tim_reg->BDTR |= 1UL << 15; // Enable main output
    } else if (pwm->tim_reg == TIM3) {
        RCC->APBENR1 |= 1UL << 1;
    }

    pwm->tim_reg->PSC = FREQ / 1000000 - 1; // Divide clock to 1 MHz

    // Timer count to value
    pwm->tim_reg->ARR = (uint32_t)((1.0f / pwm->frequency) * 1000000);

    if (pwm->channel == 4) {
        pwm->tim_reg->CCMR2 |= 3UL << 13; // PWM Mode 1 (OC4M = 0110)
        pwm->tim_reg->CCER |= 1UL << 12; // Enable output on CH4 (CC4E = 1)
    } else if (pwm->channel == 2) {
        pwm->tim_reg->CCMR1 |= 3UL << 13;
        pwm->tim_reg->CCER |= 1UL << 4;
    }

    // Enable the clock counter
    pwm->tim_reg->CR1 |= 1UL;
}

void pwm_set_pulse(pwm_t *pwm, uint16_t pulse_us) {
    if (pwm->channel == 2) {
        pwm->tim_reg->CCR2 = pulse_us;
    } else if (pwm->channel == 4) {
        pwm->tim_reg->CCR4 = pulse_us;
    }
}
