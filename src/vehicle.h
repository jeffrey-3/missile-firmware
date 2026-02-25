#ifndef VEHICLE_H
#define VEHICLE_H

#include "hal/clock.h"
#include "hal/pwm.h"
#include "hal/uart.h"
#include "ins.h"
#include "logger.h"
#include "control.h"
#include "indicator.h"
#include "board.h"

typedef struct {
    ins_t ins;
    logger_t logger;
    control_t control;
    indicator_t indicator;
    spi_t imu_spi;
    spi_t flash_spi;
    pwm_t servo_y;
    pwm_t servo_z;
    uart_t *debug_uart;
    uint32_t ins_timer;
} vehicle_t;

void vehicle_init(vehicle_t *vehicle, uart_t *debug_uart);
void vehicle_update(vehicle_t *vehicle);

#endif // VEHICLE_H
