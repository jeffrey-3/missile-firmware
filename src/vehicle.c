#include "vehicle.h"

void vehicle_init(vehicle_t *vehicle) {
    vehicle->led_timer = 0;
    vehicle->ins_timer = 0;
    vehicle->counter = 0;
    vehicle->led_on = false;

    vehicle->imu_spi.spi_reg = SPI1;
    vehicle->imu_spi.cs = board_pins.spi1_cs;
    vehicle->imu_spi.miso = board_pins.spi1_miso;
    vehicle->imu_spi.mosi = board_pins.spi1_mosi;
    vehicle->imu_spi.sck = board_pins.spi1_sck;

    vehicle->flash_spi.spi_reg = SPI2;
    vehicle->flash_spi.cs = board_pins.spi2_cs;
    vehicle->flash_spi.miso = board_pins.spi2_miso;
    vehicle->flash_spi.mosi = board_pins.spi2_mosi;
    vehicle->flash_spi.sck = board_pins.spi2_sck;

    vehicle->debug_uart.uart_reg = UART1;
    vehicle->debug_uart.tx = board_pins.uart1_tx;
    vehicle->debug_uart.rx = board_pins.uart1_rx;
    vehicle->debug_uart.baud = 115200;

    systick_init();
    gpio_init(&board_pins.led);
    timer_init(&vehicle->servo_y, TIM1, &board_pins.tim1_ch4);
    timer_init(&vehicle->servo_z, TIM3, &board_pins.tim3_ch2);
    uart_init(&vehicle->debug_uart);
    spi_init(&vehicle->imu_spi);
    spi_init(&vehicle->flash_spi);

    icm45686_init(&vehicle->imu, &vehicle->imu_spi);

    ins_init(&vehicle->ins);

    logger_init(&vehicle->logger, &vehicle->flash_spi, &vehicle->debug_uart);
}

void vehicle_update_flight(vehicle_t *vehicle) {
    if (timer_expired(&vehicle->led_timer, 500)) {
        gpio_write(&board_pins.led, vehicle->led_on);
        vehicle->led_on = !vehicle->led_on;
    }

    if (timer_expired(&vehicle->ins_timer, 10)) {
        icm45686_read_accel(&vehicle->imu, vehicle->accel);
        icm45686_read_gyro(&vehicle->imu, vehicle->gyro);

        ins_update(&vehicle->ins, vehicle->gyro[0], vehicle->gyro[1],
            vehicle->gyro[2], vehicle->accel[0], vehicle->accel[1],
            vehicle->accel[2], 0.01f);

        vehicle->counter++;
        message_t message = {
            .counter = vehicle->counter,
            .time = get_time(),
            .gx = vehicle->gyro[0],
            .gy = vehicle->gyro[1],
            .gz = vehicle->gyro[2],
            .ax = vehicle->accel[0],
            .ay = vehicle->accel[1],
            .az = vehicle->accel[2]
        };
        logger_write(&vehicle->logger, message);

        vehicle_print_state(vehicle);
    }
}

void vehicle_update_calibrate(vehicle_t *vehicle) {
    if (timer_expired(&vehicle->led_timer, 500)) {
        gpio_write(&board_pins.led, vehicle->led_on);
        vehicle->led_on = !vehicle->led_on;
    }
}

void vehicle_update_retreive(vehicle_t *vehicle) {
    logger_read_output(&vehicle->logger);
    for (;;) spin(1);
}

void vehicle_update_erase(vehicle_t *vehicle) {
    logger_erase_output(&vehicle->logger);
    for (;;) spin(1);
}

boot_mode_t vehicle_run_cli(vehicle_t *vehicle) {
    uint8_t cmd_buf_len = 32;
    char cmd_buf[cmd_buf_len];
    uint8_t idx = 0;
    uint32_t print_timer = 0;
    uint32_t start_time = get_time();

    for (;;) {
        while (!uart_read_ready(&vehicle->debug_uart)) {
            // Default to flight mode after 10 seconds
            if (get_time() - start_time > 10000) {
                return BOOT_MODE_FLIGHT;
            }

            if (timer_expired(&print_timer, 2000)) {
                char uart_buf[100] = "Missile CLI\r\n"
                    "(1) Flight\r\n(2) Calibrate\r\n(3) Fin Test\r\n"
                    "(4) HITL\r\n(5) Retrieve\r\n(9) Erase\r\n";
                uart_write_buf(&vehicle->debug_uart, uart_buf,
                    strlen(uart_buf));
            }
        }

        char c = uart_read_byte(&vehicle->debug_uart);

        if (c == '\r' || c == '\n') {
            cmd_buf[idx] = '\0';
            idx = 0;

            if (strcmp(cmd_buf, "1") == 0) {
                uart_write_buf(&vehicle->debug_uart, "Flight Selected\r\n", 17);
                return BOOT_MODE_FLIGHT;
            } else if (strcmp(cmd_buf, "2") == 0) {
                uart_write_buf(&vehicle->debug_uart, "Calibrate Selected\r\n",
                    20);
                return BOOT_MODE_CALIBRATE;
            } else if (strcmp(cmd_buf, "5") == 0) {
                uart_write_buf(&vehicle->debug_uart, "Retrieve Selected\r\n",
                    19);
                return BOOT_MODE_RETREIVE;
            } else if (strcmp(cmd_buf, "9") == 0) {
                uart_write_buf(&vehicle->debug_uart, "Erase Selected\r\n", 16);
                return BOOT_MODE_ERASE;
            } else {
                uart_write_buf(&vehicle->debug_uart, "Invalid\r\n", 9);
            }

            continue;
        }

        if (idx < (cmd_buf_len - 1)) {
            cmd_buf[idx++] = c;
        } else {
            idx = 0;
        }
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
    uart_write_buf(&vehicle->debug_uart, uart_buf, strlen(uart_buf));
}
