#include "vehicle.h"

static void vehicle_update_ground(vehicle_t *vehicle) {
    indicator_update_slow(&vehicle->indicator);
    estimator_update(&vehicle->estimator);

    if (vehicle->estimator.accel[2] > 2) {
        vehicle->accel_thresh_counter++;
    } else {
        vehicle->accel_thresh_counter = 0;
    }

    if (vehicle->accel_thresh_counter > 20) {
        vehicle->state = STATE_FLIGHT;
    }
}

static void vehicle_update_flight(vehicle_t *vehicle) {
    indicator_update_fast(&vehicle->indicator);
    control_update(&vehicle->control, &vehicle->estimator);
    estimator_update(&vehicle->estimator);
    logger_update(&vehicle->logger, &vehicle->estimator);
}

void vehicle_init(vehicle_t *vehicle) {
    vehicle->state = STATE_GROUND;
    vehicle->accel_thresh_counter = 0;

    systick_init();
    pwm_init(&vehicle->servo_y, TIM1, &board_pins.tim1_ch4, 4, 333.0f);
    pwm_init(&vehicle->servo_z, TIM3, &board_pins.tim3_ch2, 2, 333.0f);
    spi_init(&vehicle->imu_spi, SPI1, &board_pins.spi1_cs,
        &board_pins.spi1_miso, &board_pins.spi1_mosi, &board_pins.spi1_sck);
    spi_init(&vehicle->flash_spi, SPI2, &board_pins.spi2_cs,
        &board_pins.spi2_miso, &board_pins.spi2_mosi, &board_pins.spi2_sck);

    indicator_init(&vehicle->indicator, &board_pins.led);
    estimator_init(&vehicle->estimator, &vehicle->imu_spi);
    logger_init(&vehicle->logger, &vehicle->flash_spi);
    control_init(&vehicle->control, &vehicle->servo_y, &vehicle->servo_z);
}

void vehicle_update(vehicle_t *vehicle) {
    switch (vehicle->state) {
        case STATE_GROUND:
            vehicle_update_ground(vehicle);
            break;
        case STATE_FLIGHT:
            vehicle_update_flight(vehicle);
            break;
    }
}
