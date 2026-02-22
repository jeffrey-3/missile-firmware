#include "spi.h"

void spi_init(spi_t *spi, struct spi_reg *spi_reg, gpio_t *cs, gpio_t *miso,
    gpio_t *mosi, gpio_t *sck) {
    spi->spi_reg = spi_reg;
    spi->cs = cs;
    spi->miso = miso;
    spi->mosi = mosi;
    spi->sck = sck;

    gpio_init(spi->cs);
    gpio_init(spi->miso);
    gpio_init(spi->mosi);
    gpio_init(spi->sck);

    gpio_write(spi->cs, true);

    if (spi->spi_reg == SPI1) {
        RCC->APBENR2 |= 1UL << 12;
    } else if (spi->spi_reg == SPI2) {
        RCC->APBENR1 |= 1UL << 14;
    }

    spi->spi_reg->CR1 |= 1UL << 2; // Set as master
    spi->spi_reg->CR1 |= 1UL << 9; // Enable SSM to set NSS input manually
    spi->spi_reg->CR1 |= 1UL << 8; // Keep NSS input high so STM receives data
    spi->spi_reg->CR1 |= 1UL << 1; // Set CPOL to high when idle
    spi->spi_reg->CR1 |= 1UL << 0; // Set CPHA to 1
    spi->spi_reg->CR1 |= 1UL << 5; // Set clock speed to CPU_FREQ / 32
    spi->spi_reg->CR2 |= 1UL << 12; // Trigger RXNE at 8-bit FIFO
    spi->spi_reg->CR1 |= 1UL << 6; // SPI peripheral enable
}

void spi_transfer(spi_t *spi, const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    for (size_t i = 0; i < len; i++) {
        // Wait until TXE is set
        while (!(spi->spi_reg->SR & (1UL << 1))) spin(1);

        *(volatile uint8_t *)&spi->spi_reg->DR = tx_buf[i];

        // Wait until RXNE is set
        while (!(spi->spi_reg->SR & 1UL)) spin(1);

        rx_buf[i] = *(volatile uint8_t *)&spi->spi_reg->DR;
    }
}
