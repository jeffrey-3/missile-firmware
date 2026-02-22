#include "vehicle.h"

typedef enum {
    BOOT_MODE_FLIGHT,
    BOOT_MODE_CALIBRATE,
    BOOT_MODE_RETRIEVE,
    BOOT_MODE_ERASE
} boot_mode_t;

boot_mode_t run_cli(uart_t *uart) {
    uint8_t cmd_buf_len = 32;
    char cmd_buf[cmd_buf_len];
    uint8_t idx = 0;
    uint32_t print_timer = 0;
    uint32_t start_time = get_time();

    for (;;) {
        while (!uart_read_ready(uart)) {
            // Default to flight mode after 10 seconds
            if (get_time() - start_time > 10000) {
                return BOOT_MODE_FLIGHT;
            }

            if (timer_expired(&print_timer, 2000)) {
                char uart_buf[100] = "Missile CLI\r\n"
                    "(1) Flight\r\n(2) Calibrate\r\n(3) Fin Test\r\n"
                    "(4) HITL\r\n(5) Retrieve\r\n(9) Erase\r\n";
                uart_write_buf(uart, uart_buf, strlen(uart_buf));
            }
        }

        char c = uart_read_byte(uart);

        if (c == '\r' || c == '\n') {
            cmd_buf[idx] = '\0';
            idx = 0;

            if (strcmp(cmd_buf, "1") == 0) {
                uart_write_buf(uart, "Flight Selected\r\n", 17);
                return BOOT_MODE_FLIGHT;
            } else if (strcmp(cmd_buf, "2") == 0) {
                uart_write_buf(uart, "Calibrate Selected\r\n", 20);
                return BOOT_MODE_CALIBRATE;
            } else if (strcmp(cmd_buf, "5") == 0) {
                uart_write_buf(uart, "Retrieve Selected\r\n", 19);
                return BOOT_MODE_RETRIEVE;
            } else if (strcmp(cmd_buf, "9") == 0) {
                uart_write_buf(uart, "Erase Selected\r\n", 16);
                return BOOT_MODE_ERASE;
            } else {
                uart_write_buf(uart, "Invalid\r\n", 9);
            }

            continue;
        }

        if (idx < (cmd_buf_len - 1)) {
            cmd_buf[idx++] = c;
        } else {
            idx = 0;
        }
    }
}

int main(void) {
    vehicle_t vehicle;
    uart_t debug_uart;

    uart_init(&debug_uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx,
        115200);

    vehicle_init(&vehicle, &debug_uart);

    switch (run_cli(&debug_uart)) {
        case BOOT_MODE_FLIGHT:
            for (;;) {
                vehicle_update(&vehicle);
            }
            break;
        case BOOT_MODE_CALIBRATE:
            vehicle_calibrate(&vehicle);
            break;
        case BOOT_MODE_RETRIEVE:
            vehicle_retrieve(&vehicle);
            break;
        case BOOT_MODE_ERASE:
            vehicle_erase(&vehicle);
            break;
    }

    return 0;
}
