#ifndef SPI_H
#define SPI_H

#include <stddef.h>
#include <stdint.h>
#include "registers.h"
#include "clock.h"
#include "gpio.h"

typedef struct {
    struct spi_reg *spi_reg;
    gpio_t *cs;
    gpio_t *miso;
    gpio_t *mosi;
    gpio_t *sck;
} spi_t;

void spi_init(spi_t *spi, struct spi_reg *spi_reg, gpio_t *cs, gpio_t *miso,
    gpio_t *mosi, gpio_t *sck);
void spi_transfer(spi_t *spi, const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

#endif // SPI_H
