#ifndef BOARD_H
#define BOARD_H

#include "hal/gpio.h"
#include "hal/registers.h"

typedef struct {
    gpio_t led;
    gpio_t tim1_ch4;
    gpio_t tim3_ch2;
    gpio_t uart1_tx;
    gpio_t uart1_rx;
    gpio_t spi1_cs;
    gpio_t spi1_miso;
    gpio_t spi1_mosi;
    gpio_t spi1_sck;
    gpio_t spi2_cs;
    gpio_t spi2_miso;
    gpio_t spi2_mosi;
    gpio_t spi2_sck;
} board_pins_t;

extern board_pins_t board_pins;

#endif // BOARD_H
