#include "sim_interface.h"

uart_t sim_uart;

void sim_interface_init() {
    uart_init(&sim_uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx,
        115200);
}
