#include "sim_interface.h"

uart_t sim_uart;
sim_packet_t sim_packet = {0};

void sim_interface_init() {
    uart_init(&sim_uart, UART1, &pins.uart1_tx, &pins.uart1_rx,
        115200);
}
