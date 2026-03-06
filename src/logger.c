#include "logger.h"

void logger_init(logger_t *logger, spi_t *spi) {
    logger->flash_spi = spi;
    logger->current_page = 0;
    logger->counter = 0;
    logger->timer = 0;

    ring_buffer_setup(&logger->ring_buffer);

    w25q128jv_init(&logger->flash, logger->flash_spi);
    w25q128jv_write_enable(&logger->flash);
}

void logger_update(logger_t *logger, estimator_t *estimator) {
    if (!timer_expired(&logger->timer, 10)) return;

    message_t message = {
        .counter = logger->counter++,
        .time = get_time(),
        .gx = estimator->gyro[0],
        .gy = estimator->gyro[1],
        .gz = estimator->gyro[2],
        .ax = estimator->accel[0],
        .ay = estimator->accel[1],
        .az = estimator->accel[2]
    };

    // Add message to buffer
    uint8_t byte_array[sizeof(message)];
    memcpy(&byte_array, &message, sizeof(message));
    ring_buffer_write_arr(&logger->ring_buffer, byte_array, sizeof(message));

    if (!w25q128jv_check_busy(&logger->flash)) {
        if (w25q128jv_check_write_enabled(&logger->flash)) {
            // Check if buffer contains more than one page
            if (ring_buffer_count(&logger->ring_buffer) > W25Q128JV_PAGE_SIZE) {
                // Get one page of bytes from ring buffer
                uint8_t write_buf[W25Q128JV_PAGE_SIZE];
                ring_buffer_read_arr(&logger->ring_buffer, write_buf,
                    W25Q128JV_PAGE_SIZE);

                // Write one page
                w25q128jv_write_page(&logger->flash, logger->current_page, 0,
                    W25Q128JV_PAGE_SIZE, write_buf);
                logger->current_page++;
            }
        } else {
            // After every write, chip disables write, so must re-enable
            w25q128jv_write_enable(&logger->flash);
        }
    }
}
