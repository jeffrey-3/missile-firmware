#ifndef SIM_INTERFACE_H
#define SIM_INTERFACE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/hal/uart.h"
#include "../../src/pins.h"

typedef struct {
    float gyro[3];
    float accel[3];
} sim_receive_t;

typedef struct {
    uint16_t servo_y;
    uint16_t servo_z;
} sim_transmit_t;

extern sim_receive_t sim_receive;
extern sim_transmit_t sim_transmit;

typedef struct {
    uart_t uart;
    char buffer[200];
    uint8_t buffer_index;
} sim_interface_t;

void sim_interface_init(sim_interface_t *sim);
void sim_interface_update(sim_interface_t *sim);

#endif // SIM_INTERFACE_H
