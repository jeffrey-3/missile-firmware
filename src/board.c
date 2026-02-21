#include "board.h"

const board_pins_t board_pins = {
    .led = {
        .gpio_reg = GPIOB,
        .pin = 9,
        .af = 0,
        .mode = GPIO_MODE_OUTPUT
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

static spi_t icm45686_spi = {
    .spi_reg = SPI1,
    .cs = board_pins.spi1_cs,
    .miso = board_pins.spi1_miso,
    .mosi = board_pins.spi1_mosi,
    .sck = board_pins.spi1_sck
}

static spi_t w25q128jv_spi = {
    .spi_reg = SPI2,
    .cs = board_pins.spi2_cs,
    .miso = board_pins.spi2_miso,
    .mosi = board_pins.spi2_mosi,
    .sck = board_pins.spi2_sck
}

static uart_t debug_uart = {
    .uart_reg = UART1,
    .tx = board_pins.uart1_tx,
    .rx = board_pins.uart1_rx,
    .baud = 115200
}

void board_init(void) {
    systick_init();
    gpio_init(board_pins.led);
    uart_init(&debug_uart);
    spi_init(&icm45686_spi);
    spi_init(&w25q128jv_spi);
}

void board_icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(&icm45686_spi.cs, false);
    spi_transfer_buf(&icm45686_spi, tx_buf, rx_buf, len);
    gpio_write(&icm45686.cs, true);
}

void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(&w25q128jv_spi.cs, false);
    spi_transfer_buf(&w25q128jv_spi, tx_buf, rx_buf, len);
    gpio_write(&w25q128jv_spi.cs, true);
}
