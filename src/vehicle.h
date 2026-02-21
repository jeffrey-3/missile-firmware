#ifndef VEHICLE_H
#define VEHICLE_H

#include "board.h"
#include "math/math.h"
#include "peripherals/icm45686.h"
#include "peripherals/w25q128jv.h"
#include "math/quaternion.h"
#include "ins.h"
#include "logger.h"

typedef enum {
    BOOT_MODE_FLIGHT,
    BOOT_MODE_CALIBRATE,
    BOOT_MODE_RETREIVE,
    BOOT_MODE_ERASE
} boot_mode_t;

typedef struct {
    ins_t ins;
    logger_t logger;
    icm45686_t imu;
    w25q128jv_t flash;
    bool led_on;
    uint32_t led_timer;
    uint32_t ins_timer;
    uint32_t counter;
    float accel[3];
    float gyro[3];
} vehicle_t;

void vehicle_init(vehicle_t *vehicle);
void vehicle_imu_init(vehicle_t *vehicle);
void vehicle_flash_init(vehicle_t *vehicle);
void vehicle_ins_init(vehicle_t *vehicle);
void vehicle_logger_init(vehicle_t *vehicle);

void vehicle_update(vehicle_t *vehicle);
void vehicle_update_flight(vehicle_t *vehicle);
void vehicle_update_calibrate(vehicle_t *vehicle);
void vehicle_update_retreive(vehicle_t *vehicle);
void vehicle_update_erase(vehicle_t *vehicle);

boot_mode_t vehicle_run_cli();
void vehicle_print_state(vehicle_t *vehicle);

#endif // VEHICLE_H
