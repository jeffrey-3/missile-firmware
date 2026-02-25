#include "logger.h"

static uint8_t data_buffer[LOGGER_RING_BUF_SIZE] = {0};

void logger_init(logger_t *logger, spi_t *spi) {
    logger->flash_spi = spi;
    logger->current_page = 0;
    logger->counter = 0;
    logger->timer = 0;

    ring_buffer_setup(&logger->ring_buffer, data_buffer, LOGGER_RING_BUF_SIZE);

    w25q128jv_init(&logger->flash, logger->flash_spi);

    w25q128jv_write_enable(&logger->flash);
    delay(LOGGER_WRITE_EN_TIME);
}

/*
 * @brief Write buffer to memory
 *
 * Write may take some time to complete at the hardware level, so do not call
 * again before the last write is complete
 */
void logger_update(logger_t *logger, ins_t *ins) {
    if (!timer_expired(&logger->timer, 10)) return;

    message_t message = {
        .counter = logger->counter++,
        .time = get_time(),
        .gx = ins->gyro[0],
        .gy = ins->gyro[1],
        .gz = ins->gyro[2],
        .ax = ins->accel[0],
        .ay = ins->accel[1],
        .az = ins->accel[2]
    };

    // Add message to buffer
    uint8_t byte_array[sizeof(message)];
    memcpy(&byte_array, &message, sizeof(message));

    for (uint32_t i = 0; i < sizeof(message); i++) {
        ring_buffer_write(&logger->ring_buffer, byte_array[i]);
    }

    // Check if buffer is ready to flush
    uint32_t page_size = LOGGER_MSG_PER_PAGE * sizeof(message_t);
    if (ring_buffer_count(&logger->ring_buffer) > page_size) {
        // Get data from ring buffer
        uint8_t write_buf[page_size];

        for (uint32_t i = 0; i < page_size; i++) {
            ring_buffer_read(&logger->ring_buffer, &write_buf[i]);
        }

        // Write the data
        w25q128jv_write_page(&logger->flash, logger->current_page, 0,
            LOGGER_MSG_PER_PAGE * sizeof(message_t), write_buf);
        logger->current_page++;
    } else {
        // After every write, the flash chip disables write, so must re-enable
        w25q128jv_write_enable(&logger->flash);
    }
}
