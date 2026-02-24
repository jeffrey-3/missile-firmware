#ifndef VEHICLE_H
#define VEHICLE_H

#include "peripherals/icm45686.h"
#include "math/math.h"
#include "math/quaternion.h"
#include "hal/clock.h"
#include "hal/gpio.h"
#include "hal/timer.h"
#include "hal/uart.h"
#include "hal/spi.h"
#include "ins.h"
#include "logger.h"
#include "control.h"
#include "board.h"

typedef struct {
    ins_t ins;
    logger_t logger;
    control_t control;
    spi_t imu_spi;
    spi_t flash_spi;
    timer_t servo_y;
    timer_t servo_z;
    uart_t *debug_uart;
    bool led_on;
    uint32_t led_timer;
    uint32_t ins_timer;
} vehicle_t;

void vehicle_init(vehicle_t *vehicle, uart_t *debug_uart);
void vehicle_update(vehicle_t *vehicle);
void vehicle_print_state(vehicle_t *vehicle);

#endif // VEHICLE_H
