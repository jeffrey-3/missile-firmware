#include "sim_interface.h"

sim_interface_t sim = {0};
vehicle_t vehicle = {0};
sim_receive_t sim_receive = {0};
sim_transmit_t sim_transmit = {0};

void sim_interface_init(sim_interface_t *s) {
    vehicle.estimator.launched = true;
    sim_transmit.servo_y = 1500;
    sim_transmit.servo_z = 1500;
    uart_init(&s->uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 1000000);
}
