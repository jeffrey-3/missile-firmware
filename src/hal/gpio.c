#include "gpio.h"

void gpio_set_mode(gpio_t *gpio, gpio_mode_t mode) {
    struct gpio_registers *gpio_regs = GPIO(gpio->bank - 'A'); // GPIO bank
    RCC->IOPENR |= BIT(gpio->bank - 'A'); // Enable GPIO clock
    gpio_regs->MODER &= ~(3U << (gpio->number * 2)); // Clear existing
    gpio_regs->MODER |= (mode & 3) << (gpio->number * 2); // Set new mode
}

void gpio_write(gpio_t *gpio, bool val) {
    struct gpio_registers *gpio_regs = GPIO(gpio->bank - 'A');
    gpio_regs->BSRR = (1U << gpio->number) << (val ? 0 : 16);
}

void gpio_set_af(gpio_t * gpio) {
    struct gpio_registers *gpio_regs = GPIO(gpio->bank - 'A'); // GPIO bank
    int n = gpio->number;
    gpio_regs->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio_regs->AFR[n >> 3] |= ((uint32_t) gpio->af) << ((n & 7) * 4);
}
