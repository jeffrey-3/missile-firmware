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
#include "board.h"

typedef enum {
    BOOT_MODE_FLIGHT,
    BOOT_MODE_CALIBRATE,
    BOOT_MODE_RETREIVE,
    BOOT_MODE_ERASE
} boot_mode_t;

typedef struct {
    ins_t ins;
    logger_t logger;
    spi_t imu_spi;
    spi_t flash_spi;
    uart_t debug_uart;
    timer_t servo_y;
    timer_t servo_z;
    bool led_on;
    uint32_t led_timer;
    uint32_t ins_timer;
} vehicle_t;

void vehicle_init(vehicle_t *vehicle);

void vehicle_update_flight(vehicle_t *vehicle);
void vehicle_update_calibrate(vehicle_t *vehicle);
void vehicle_update_retreive(vehicle_t *vehicle);
void vehicle_update_erase(vehicle_t *vehicle);

boot_mode_t vehicle_run_cli(vehicle_t *vehicle);
void vehicle_print_state(vehicle_t *vehicle);

#endif // VEHICLE_H
