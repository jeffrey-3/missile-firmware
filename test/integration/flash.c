#include <string.h>
#include "../../src/peripherals/w25q128jv.h"
#include "../../src/hal/uart.h"
#include "../../src/logger.h"
#include "../../src/pins.h"

void test_erase_flash(void) {
    spi_t spi;
    uart_t uart;
    w25q128jv_t flash;

    uart_init(&uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
    spi_init(&spi, SPI2, &pins.spi2_cs, &pins.spi2_miso, &pins.spi2_mosi,
        &pins.spi2_sck);
    w25q128jv_init(&flash, &spi);

    // Erase every sector one by one
    for (uint16_t i = 0; i < W25Q128JV_NUM_SECTORS; i++) {
        while (w25q128jv_check_busy(&flash)) spin(1);
        w25q128jv_write_enable(&flash);

        while (w25q128jv_check_busy(&flash)) spin(1);
        w25q128jv_erase_sector(&flash, i);

        char uart_buf[100];
        snprintf(uart_buf, sizeof(uart_buf), "Erased %d out of %d\r\n",
            i + 1, W25Q128JV_NUM_SECTORS);
        uart_write(&uart, uart_buf, strlen(uart_buf));
    }

    char uart_buf[100] = "Finished\r\n";
    uart_write(&uart, uart_buf, strlen(uart_buf));
}

void test_read_flash(void) {
    spi_t spi;
    uart_t uart;
    w25q128jv_t flash;
    rb_u8_t ring_buffer;

    rb_setup_u8(&ring_buffer);
    uart_init(&uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
    spi_init(&spi, SPI2, &pins.spi2_cs, &pins.spi2_miso, &pins.spi2_mosi,
        &pins.spi2_sck);
    w25q128jv_init(&flash, &spi);

    // Read each page one by one
    uint32_t num_pages = W25Q128JV_PAGES_PER_SECTOR * W25Q128JV_NUM_SECTORS;
    for (uint32_t i = 0; i < num_pages; i++) {
        char buf[100];
        snprintf(buf, sizeof(buf), "Reading page %ld out of %ld\r\n", i + 1,
            num_pages);
        uart_write(&uart, buf, strlen(buf));

        // Read this page and get array of message structs
        while (w25q128jv_check_busy(&flash)) spin(1);
        w25q128jv_write_disable(&flash);

        uint8_t data[W25Q128JV_PAGE_SIZE];
        while (w25q128jv_check_busy(&flash)) spin(1);
        w25q128jv_read(&flash, i, 0, W25Q128JV_PAGE_SIZE, data);
        rb_write_u8(&ring_buffer, data, W25Q128JV_PAGE_SIZE);

        while (rb_count_u8(&ring_buffer) > sizeof(message_t)) {
            uint8_t message_bytes[sizeof(message_t)];
            rb_read_u8(&ring_buffer, message_bytes, sizeof(message_t));

            message_t message;
            memcpy(&message, message_bytes, sizeof(message_t));

            // Print contents of message
            char uart_buf[100];
            snprintf(uart_buf, sizeof(uart_buf),
                "%ld,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
                message.counter, message.time, (double)message.gx,
                (double)message.gy, (double)message.gz, (double)message.ax,
                (double)message.ay, (double)message.az);
            uart_write(&uart, uart_buf, strlen(uart_buf));
        }
    }

    uart_write(&uart, "Finished\r\n", strlen("Finished\r\n"));
}
