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

typedef enum {
    STATE_GROUND,
    STATE_FLIGHT,
} state_t;

typedef struct {
    ins_t ins;
    logger_t logger;
    control_t control;
    indicator_t indicator;
    spi_t imu_spi;
    spi_t flash_spi;
    pwm_t servo_y;
    pwm_t servo_z;
    state_t state;
    uint16_t accel_thresh_counter;
} vehicle_t;

void vehicle_init(vehicle_t *vehicle);
void vehicle_update(vehicle_t *vehicle);

#endif // VEHICLE_H
