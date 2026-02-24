#include "vehicle.h"

int main(void) {
    uart_t debug_uart;
    vehicle_t vehicle;

    systick_init();
    uart_init(&debug_uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx,
        115200);
    vehicle_init(&vehicle, &debug_uart);

    for (;;) {
        vehicle_update(&vehicle);
    }

    return 0;
}
