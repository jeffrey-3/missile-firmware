#ifndef SPI_H
#define SPI_H

#include <stddef.h>
#include <stdint.h>
#include "registers.h"
#include "clock.h"

typedef struct {
    struct spi_reg *spi_reg;
} spi_t;

void spi_init(spi_t *spi);
uint8_t spi_transfer(spi_t *spi, uint8_t tx_data);
void spi_transfer_buf(spi_t *spi, const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len);

#endif // SPI_H
