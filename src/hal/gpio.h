#ifndef GPIO_H
#define GPIO_H

enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
};

typedef struct {
    struct gpio_reg *gpio_reg;
    uint8_t pin;
    uint8_t af;
    uint8_t mode;
} gpio_t;

void gpio_init(gpio_t *gpio);
void gpio_write(gpio_t *gpio, bool val);

#endif // GPIO_H
