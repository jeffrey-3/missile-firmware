#include "spi.h"

void spi_init(spi_t *spi) {
    if (spi->spi_reg == SPI1) {
        RCC->APBENR2 |= BIT(12);
    } else if (spi->spi_reg == SPI2) {
        RCC->APBENR1 |= BIT(14);
    }

    spi->spi_reg->CR1 |= BIT(2); // Set as master
    spi->spi_reg->CR1 |= BIT(9); // Enable SSM to set NSS input manually
    spi->spi_reg->CR1 |= BIT(8); // Keep NSS input high so STM receives data
    spi->spi_reg->CR1 |= BIT(1); // Set CPOL to high when idle
    spi->spi_reg->CR1 |= BIT(0); // Set CPHA to 1
    spi->spi_reg->CR1 |= BIT(5); // Set clock speed to CPU_FREQ / 32
    spi->spi_reg->CR2 |= BIT(12); // Trigger RXNE at 8-bit FIFO
    spi->spi_reg->CR1 |= BIT(6); // SPI peripheral enable
}

uint8_t spi_transfer(spi_t *spi, uint8_t tx_data) {
    // Wait until TXE is set
    while (!(spi->spi_reg->SR & BIT(1))) spin(1);

    *(volatile uint8_t *)&spi->spi_reg->DR = tx_data;

    // Wait until RXNE is set
    while (!(spi->spi_reg->SR & BIT(0))) spin(1);

    return *(volatile uint8_t *)&spi->spi_reg->DR;
}

void spi_transfer_buf(spi_t *spi, const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    for (size_t i = 0; i < len; i++) {
        rx_buf[i] = spi_transfer(spi, tx_buf[i]);
    }
}
