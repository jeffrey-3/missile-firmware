#ifndef SIM_INTERFACE_H
#define SIM_INTERFACE_H

#include "../../src/hal/uart.h"
#include "../../src/pins.h"

typedef struct __attribute__((packed)) {
    uint32_t time;
    float gyro[3];
    float accel[3];
} sim_packet_t;

extern uart_t sim_uart;
extern sim_packet_t sim_packet;

void sim_interface_init();

#endif // SIM_INTERFACE_H
