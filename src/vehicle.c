#include "vehicle.h"

void vehicle_init(vehicle_t *vehicle, uart_t *debug_uart) {
    vehicle->debug_uart = debug_uart;
    vehicle->ins_timer = 0;

    pwm_init(&vehicle->servo_y, TIM1, &board_pins.tim1_ch4, 4, 333.0f);
    pwm_init(&vehicle->servo_z, TIM3, &board_pins.tim3_ch2, 2, 333.0f);
    spi_init(&vehicle->imu_spi, SPI1, &board_pins.spi1_cs,
        &board_pins.spi1_miso, &board_pins.spi1_mosi, &board_pins.spi1_sck);
    spi_init(&vehicle->flash_spi, SPI2, &board_pins.spi2_cs,
        &board_pins.spi2_miso, &board_pins.spi2_mosi, &board_pins.spi2_sck);

    indicator_init(&vehicle->indicator, &board_pins.led);
    ins_init(&vehicle->ins, &vehicle->imu_spi);
    logger_init(&vehicle->logger, &vehicle->flash_spi, vehicle->debug_uart);
    control_init(&vehicle->control, &vehicle->servo_y, &vehicle->servo_z);
}

void vehicle_update(vehicle_t *vehicle) {
    if (timer_expired(&vehicle->ins_timer, 10)) {
        indicator_update_slow(&vehicle->indicator);
        ins_update(&vehicle->ins, 0.01f);
        control_update(&vehicle->control, &vehicle->ins);
        logger_write(&vehicle->logger, &vehicle->ins);
        vehicle_print_state(vehicle);
    }
}

void vehicle_print_state(vehicle_t *vehicle) {
    float roll;
    float pitch;
    float yaw;
    char uart_buf[64];

    quat_to_euler(vehicle->ins.q, &roll, &pitch, &yaw);

    roll *= RAD2DEG;
    pitch *= RAD2DEG;
    yaw *= RAD2DEG;

    snprintf(uart_buf, sizeof(uart_buf),
        "%.0f,%.0f,%.0f,%.1f,%.1f,%.1f\r\n",
        (double)roll, (double)pitch, (double)yaw,
        (double)vehicle->ins.vel.x, (double)vehicle->ins.vel.y,
        (double)vehicle->ins.vel.z);
    uart_write_buf(vehicle->debug_uart, uart_buf, strlen(uart_buf));
}
