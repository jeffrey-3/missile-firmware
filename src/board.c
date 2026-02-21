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

void board_init(void) {
    systick_init();

    board_setup_led();
    board_setup_uart1();
    board_setup_spi1();
    board_setup_spi2();
}

void board_setup_led(void) {
    gpio_init(board_pins.led);
}

void board_setup_uart1(void) {
    gpio_init(board_pins.uart1_tx);
    gpio_init(board_pins.uart1_rx);

    uart_init(UART1, 115200);
}

void board_setup_spi1(void) {
    spi_init(&icm45686_spi);
}

void board_setup_spi2(void) {
    gpio_init(board_pins.spi2_cs);
    gpio_write(board_pins.spi2_cs, true);

    gpio_init(board_pins.spi2_miso);
    gpio_init(board_pins.spi2_mosi);
    gpio_init(board_pins.spi2_sck);

    spi_init(SPI2);
}

void board_icm45686_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.spi1_cs, false);
    spi_transfer_buf(SPI1, tx_buf, rx_buf, len);
    gpio_write(board_pins.spi1_cs, true);
}

void board_w25q128jv_spi_transfer(const uint8_t *tx_buf, uint8_t *rx_buf,
    size_t len) {
    gpio_write(board_pins.spi2_cs, false);
    spi_transfer_buf(SPI2, tx_buf, rx_buf, len);
    gpio_write(board_pins.spi2_cs, true);
}
