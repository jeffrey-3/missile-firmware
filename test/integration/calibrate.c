#include <string.h>
#include <stdio.h>
#include "../../src/peripherals/icm45686.h"
#include "../../src/hal/uart.h"
#include "../../src/pins.h"

void test_calibrate(void) {
    uart_t uart;
    spi_t spi;
    icm45686_t imu;
    uint32_t timer;

    uart_init(&uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
    spi_init(&spi, SPI1, &pins.spi1_cs, &pins.spi1_miso, &pins.spi1_mosi,
        &pins.spi1_sck);
    icm45686_init(&imu, &spi);

    for (;;) {
        if (timer_expired(&timer, 20)) {
            float accel[3], gyro[3];
            icm45686_read_accel(&imu, accel);
            icm45686_read_gyro(&imu, gyro);

            char uart_buf[200];
            snprintf(uart_buf, sizeof(uart_buf),
                "%f,%f,%f\r\n",
                (double)accel[0], (double)accel[1], (double)accel[2]);
            uart_write(&uart, uart_buf, strlen(uart_buf));
        }
    }
}
