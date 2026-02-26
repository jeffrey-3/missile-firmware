#include "../../src/peripherals/icm45686.h"

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
    (void)data;
}

void icm45686_read_gyro(icm45686_t *device, float *data) {
    (void)device;
    (void)data;
}
