#include <string.h>
#include <stdio.h>
#include "src/estimator.h"
#include "../../src/hal/uart.h"
#include "../../src/pins.h"

void test_estimator() {
    uart_t uart;
    spi_t spi;
    estimator_t estimator;
    uint32_t timer = 0;

    uart_init(&uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
    spi_init(&spi, SPI1, &pins.spi1_cs, &pins.spi1_miso, &pins.spi1_mosi,
        &pins.spi1_sck);
    estimator_init(&estimator, &spi);

    for (;;) {
        estimator_update(&estimator);

        if (timer_expired(&timer, 10)) {
            float roll, pitch, yaw;
            quat_to_euler(estimator.q, &roll, &pitch, &yaw);
            roll *= RAD2DEG;
            pitch *= RAD2DEG;
            yaw *= RAD2DEG;

            char uart_buf[200];
            snprintf(uart_buf, sizeof(uart_buf), "%.1f %.1f %.1f %d\r\n", (double)roll,
                (double)pitch, (double)yaw, estimator.launched);
            uart_write(&uart, uart_buf, strlen(uart_buf));
        }

    }
}
