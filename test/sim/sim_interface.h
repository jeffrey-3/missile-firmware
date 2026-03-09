#ifndef SIM_INTERFACE_H
#define SIM_INTERFACE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../src/vehicle.h"
#include "../../src/hal/uart.h"
#include "../../src/pins.h"

extern vehicle_t vehicle;

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
} sim_interface_t;

extern sim_interface_t sim;

void sim_interface_init(sim_interface_t *s);

#endif // SIM_INTERFACE_H
