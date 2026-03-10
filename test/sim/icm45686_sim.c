#include "../../src/peripherals/icm45686.h"
#include "sim_interface.h"

void icm45686_init(icm45686_t *device, spi_t *spi) {
    (void)device;
    (void)spi;
}

uint8_t icm45686_read_id(icm45686_t *device) {
    (void)device;
    return 0;
}

void icm45686_read_accel(icm45686_t *device, float *data) {
    (void)device;
    data[0] = sim_receive.accel[0];
    data[1] = sim_receive.accel[1];
    data[2] = sim_receive.accel[2];
}

void icm45686_read_gyro(icm45686_t *device, float *data) {
    (void)device;
    // Send euler angles for debug
    float roll, pitch, yaw;
    quat_to_euler(vehicle.estimator.q, &roll, &pitch, &yaw);
    roll *= RAD2DEG;
    pitch *= RAD2DEG;
    yaw *= RAD2DEG;

    // Send time and controls to simulator
    char buf[200];
    snprintf(buf, sizeof(buf), "%ld,%d,%d,%d\r\n", get_time(),
        ESTIMATOR_DT, sim_transmit.servo_y, sim_transmit.servo_z);
    uart_write(&sim.uart, buf, strlen(buf));

    // Read data from simulator
    char buffer[400] = {0};
    uint8_t buffer_index = 0;
    char c = '\0';
    while (c != '\r') {
        if (!uart_empty(&sim.uart)) {
            uint8_t rx_byte;
            uart_read(&sim.uart, &rx_byte);
            c = (char)rx_byte;
            buffer[buffer_index++] = c;
        }
    }

    // Parse
    float values[10] = {0};
    uint8_t col = 0;
    char *token = strtok(buffer, ",");
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
    sim_receive.pitch_error = values[6];
    sim_receive.yaw_error = values[7];

    data[0] = sim_receive.gyro[0];
    data[1] = sim_receive.gyro[1];
    data[2] = sim_receive.gyro[2];
}
