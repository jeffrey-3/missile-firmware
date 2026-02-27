#include "pins.h"

pins_t pins = {
    .led = {
        .gpio_reg = GPIOB,
        .pin = 9,
        .af = 0,
        .mode = GPIO_MODE_OUTPUT
    },
    .tim1_ch4 = {
        .gpio_reg = GPIOA,
        .pin = 11,
        .af = 2,
        .mode = GPIO_MODE_AF
    },
    .tim3_ch2 = {
        .gpio_reg = GPIOA,
        .pin = 7,
        .af = 1,
        .mode = GPIO_MODE_AF
    },
    .uart1_tx = {
        .gpio_reg = GPIOB,
        .pin = 6,
        .af = 0,
        .mode = GPIO_MODE_AF
    },
    .uart1_rx = {
        .gpio_reg = GPIOB,
        .pin = 7,
        .af = 0,
        .mode = GPIO_MODE_AF
    },
    .spi1_cs = {
        .gpio_reg = GPIOA,
        .pin = 1,
        .af = 0,
        .mode = GPIO_MODE_OUTPUT
    },
    .spi1_miso = {
        .gpio_reg = GPIOA,
        .pin = 6,
        .af = 0,
        .mode = GPIO_MODE_AF
    },
    .spi1_mosi = {
        .gpio_reg = GPIOA,
        .pin = 12,
        .af = 0,
        .mode = GPIO_MODE_AF
    },
    .spi1_sck = {
        .gpio_reg = GPIOA,
        .pin = 5,
        .af = 0,
        .mode = GPIO_MODE_AF
    },
    .spi2_cs = {
        .gpio_reg = GPIOC,
        .pin = 15,
        .af = 0,
        .mode = GPIO_MODE_OUTPUT
    },
    .spi2_miso = {
        .gpio_reg = GPIOB,
        .pin = 2,
        .af = 1,
        .mode = GPIO_MODE_AF
    },
    .spi2_mosi = {
        .gpio_reg = GPIOA,
        .pin = 4,
        .af = 1,
        .mode = GPIO_MODE_AF
    },
    .spi2_sck = {
        .gpio_reg = GPIOA,
        .pin = 0,
        .af = 0,
        .mode = GPIO_MODE_AF
    }
};
