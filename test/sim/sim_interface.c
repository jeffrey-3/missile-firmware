#include "sim_interface.h"

sim_receive_t sim_receive = {0};
sim_transmit_t sim_transmit = {0};

void sim_interface_init(sim_interface_t *sim) {
    memset(sim->buffer, 0, sizeof(sim->buffer));
    sim->buffer_index = 0;
    uart_init(&sim->uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);
}

void sim_interface_update(sim_interface_t *sim) {
    char c = 0;

    while (!uart_empty(&sim->uart)) {
        uint8_t b;
        uart_read(&sim->uart, &b);
        c = (char)b;

        sim->buffer[sim->buffer_index] = c;
        sim->buffer_index++;
    }

    if (c == '\r') {
        float values[10] = {0};
        uint8_t col = 0;

        char *token = strtok(sim->buffer, ",");
        while (token != NULL) {
            values[col++] = strtof(token, NULL);
            token = strtok(NULL, ",");
        }

        sim_receive.gyro[0] = values[0];
        sim_receive.gyro[1] = values[1];
        sim_receive.gyro[2] = values[2];
        sim_receive.accel[0] = values[3];
        sim_receive.accel[1] = values[4];
        sim_receive.accel[2] = values[5];

        memset(sim->buffer, 0, sizeof(sim->buffer));
        sim->buffer_index = 0;

        char uart_buf[100];
        snprintf(uart_buf, sizeof(uart_buf), "%d, %d\r\n",
            sim_transmit.servo_y, sim_transmit.servo_z);
        uart_write(&sim->uart, uart_buf, strlen(uart_buf));
    }
}
