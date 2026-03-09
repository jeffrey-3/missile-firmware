#include "vehicle.h"

static void vehicle_update_ground(vehicle_t *vehicle) {
    indicator_update_slow(&vehicle->indicator);
    estimator_update(&vehicle->estimator);

    if (vehicle->estimator.launched) {
        vehicle->state = STATE_FLIGHT;
    }
}

static void vehicle_update_flight(vehicle_t *vehicle) {
    indicator_update_fast(&vehicle->indicator);
    estimator_update(&vehicle->estimator);
    control_update(&vehicle->control, &vehicle->estimator);
    logger_update(&vehicle->logger, &vehicle->estimator);
}

void vehicle_init(vehicle_t *vehicle) {
    vehicle->state = STATE_GROUND;

    systick_init();
    pwm_init(&vehicle->servo_y, TIM1, &pins.tim1_ch4, 4, 333.0f);
    pwm_init(&vehicle->servo_z, TIM3, &pins.tim3_ch2, 2, 333.0f);
    spi_init(&vehicle->imu_spi, SPI1, &pins.spi1_cs, &pins.spi1_miso,
        &pins.spi1_mosi, &pins.spi1_sck);
    spi_init(&vehicle->flash_spi, SPI2, &pins.spi2_cs, &pins.spi2_miso,
        &pins.spi2_mosi, &pins.spi2_sck);

    indicator_init(&vehicle->indicator, &pins.led);
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
