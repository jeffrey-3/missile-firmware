#ifndef SIM_INTERFACE_H
#define SIM_INTERFACE_H

#include "../../src/hal/uart.h"
#include "../../src/board.h"

extern uart_t sim_uart;

void sim_interface_init();

#endif // SIM_INTERFACE_H
