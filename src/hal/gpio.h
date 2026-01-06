#ifndef GPIO_H
#define GPIO_H

#include "registers.h"

typedef enum : uint8_t {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
} gpio_mode_t;

typedef struct {
    gpio_registers bank;
    uint8_t number;
    uint8_t af;
} gpio_t;

void gpio_set_mode(gpio_t *gpio, gpio_mode_t mode);
void gpio_write(gpio_t *gpio, bool val);
void gpio_set_af(gpio_t * gpio);

#endif // GPIO_H
