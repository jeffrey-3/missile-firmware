#include "gpio.h"

void gpio_init(gpio_t *gpio) {
    // Enable GPIO clock
    if (gpio->gpio_reg == GPIOA) {
        RCC->IOPENR |= 1UL;
    } else if (gpio->gpio_reg == GPIOB) {
        RCC->IOPENR |= 1UL << 1;
    } else if (gpio->gpio_reg == GPIOC) {
        RCC->IOPENR |= 1UL << 2;
    }

    // Set mode
    gpio->gpio_reg->MODER &= ~(3U << (gpio->pin * 2)); // Clear existing setting
    gpio->gpio_reg->MODER |= (gpio->mode & 3) << (gpio->pin * 2); // Set mode

    // Set alternate function
    gpio->gpio_reg->AFR[gpio->pin >> 3] &= ~(15UL << ((gpio->pin & 7) * 4));
    gpio->gpio_reg->AFR[gpio->pin >> 3] |= ((uint32_t)gpio->af) <<
        ((gpio->pin & 7) * 4);
}

void gpio_write(gpio_t *gpio, bool val) {
    gpio->gpio_reg->BSRR = (1U << gpio->pin) << (val ? 0 : 16);
}

