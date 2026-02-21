#include "vehicle.h"

static void logger_write_page(void *context, uint32_t page,
    uint8_t *data) {
    w25q128jv_t *flash = (w25q128jv_t *)context;
    w25q128jv_write_page(flash, page, 0,
        LOGGER_MSG_PER_PAGE * sizeof(message_t), data);
}

static void logger_erase_sector(void *context, uint16_t sector) {
    w25q128jv_t *flash = (w25q128jv_t *)context;
    w25q128jv_erase_sector(flash, sector);
}

static void logger_write_enable(void *context) {
    w25q128jv_t *flash = (w25q128jv_t *)context;
    w25q128jv_write_enable(flash);
}

static void logger_write_disable(void *context) {
    w25q128jv_t *flash = (w25q128jv_t *)context;
    w25q128jv_write_disable(flash);
}

static void logger_read_page(void *context, uint32_t page,
    uint8_t *data) {
    w25q128jv_t *flash = (w25q128jv_t *)context;
    w25q128jv_read(flash, page, 0, LOGGER_MSG_PER_PAGE * sizeof(message_t),
        data);
}

static void logger_output_callback(void *context, char *str,
    size_t len) {
    (void)context;
    uart_write_buf(UART1, str, len);
}

void vehicle_init(vehicle_t *vehicle) {
    vehicle->led_timer = 0;
    vehicle->ins_timer = 0;
    vehicle->counter = 0;

    board_init();

    vehicle_imu_init(vehicle);
    vehicle_flash_init(vehicle);
    vehicle_ins_init(vehicle);
    vehicle_logger_init(vehicle);
}

void vehicle_imu_init(vehicle_t *vehicle) {
    vehicle->imu.spi_transfer = board_icm45686_spi_transfer;
    icm45686_init(&vehicle->imu);
}

void vehicle_flash_init(vehicle_t *vehicle) {
    vehicle->flash.spi_transfer = board_w25q128jv_spi_transfer;
}

void vehicle_ins_init(vehicle_t *vehicle) {
    ins_init(&vehicle->ins);
}

void vehicle_logger_init(vehicle_t *vehicle) {
    vehicle->logger.context = &vehicle->flash;
    vehicle->logger.write_page = logger_write_page;
    vehicle->logger.erase_sector = logger_erase_sector;
    vehicle->logger.write_enable = logger_write_enable;
    vehicle->logger.write_disable = logger_write_disable;
    vehicle->logger.read_page = logger_read_page;
    vehicle->logger.delay_ms = delay;
    vehicle->logger.output_callback = logger_output_callback;
    logger_init(&vehicle->logger);
}

void vehicle_update_flight(vehicle_t *vehicle) {
    if (timer_expired(&vehicle->led_timer, 500)) {
        gpio_write(board_pins.led, vehicle->led_on);
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
        gpio_write(board_pins.led, vehicle->led_on);
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

boot_mode_t vehicle_run_cli() {
    uint8_t cmd_buf_len = 32;
    char cmd_buf[cmd_buf_len];
    uint8_t idx = 0;
    uint32_t print_timer = 0;
    uint32_t start_time = get_time();

    for (;;) {
        while (!uart_read_ready(UART1)) {
            // Default to flight mode after 10 seconds
            if (get_time() - start_time > 10000) {
                return BOOT_MODE_FLIGHT;
            }

            if (timer_expired(&print_timer, 2000)) {
                char uart_buf[100] = "Missile CLI\r\n"
                    "(1) Flight\r\n(2) Calibrate\r\n(3) Fin Test\r\n"
                    "(4) HITL\r\n(5) Retrieve\r\n(9) Erase\r\n";
                uart_write_buf(UART1, uart_buf, strlen(uart_buf));
            }
        }

        char c = uart_read_byte(UART1);

        if (c == '\r' || c == '\n') {
            cmd_buf[idx] = '\0';
            idx = 0;

            if (strcmp(cmd_buf, "1") == 0) {
                uart_write_buf(UART1, "Flight Selected\r\n", 17);
                return BOOT_MODE_FLIGHT;
            } else if (strcmp(cmd_buf, "2") == 0) {
                uart_write_buf(UART1, "Calibrate Selected\r\n", 20);
                return BOOT_MODE_CALIBRATE;
            } else if (strcmp(cmd_buf, "5") == 0) {
                uart_write_buf(UART1, "Retrieve Selected\r\n", 19);
                return BOOT_MODE_RETREIVE;
            } else if (strcmp(cmd_buf, "9") == 0) {
                uart_write_buf(UART1, "Erase Selected\r\n", 16);
                return BOOT_MODE_ERASE;
            } else {
                uart_write_buf(UART1, "Invalid\r\n", 9);
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
    uart_write_buf(UART1, uart_buf, strlen(uart_buf));
}
