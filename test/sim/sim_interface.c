#include "sim_interface.h"

sim_interface_t sim = {0};
vehicle_t vehicle = {0};
sim_receive_t sim_receive = {0};
sim_transmit_t sim_transmit = {0};

void sim_interface_init(sim_interface_t *s) {
    vehicle.estimator.launched = true;
    sim_transmit.servo_y = 1500;
    sim_transmit.servo_z = 1500;
    // Maybe change baud rate to 1mil to set BRR to 16
    // Then no fraction and missed bit
    // Also try 100000 or 10000
    // It will only drift if baud mismatch is too high, at beginning should
    // not fail
    // Maybe start byte? But the way its setup right now
    // should be really hard to fail.... why is it failing?
    // Print out the received data to see if its a parse issue or what
    uart_init(&s->uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
}
