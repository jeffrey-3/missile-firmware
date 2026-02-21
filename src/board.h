#ifndef BOARD_H
#define BOARD_H

#include "hal/clock.h"
#include "hal/gpio.h"
#include "hal/uart.h"
#include "hal/spi.h"

typedef struct {
    gpio_t led;
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

extern const board_pins_t board_pins;

void board_init(void);
void board_icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);
void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

#endif // BOARD_H
