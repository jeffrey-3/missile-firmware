#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hal/clock.h"
#include "hal/uart.h"
#include "hal/spi.h"
#include "peripherals/w25q128jv.h"
#include "util/ring_buffer.h"
#include "ins.h"
#include "config.h"

typedef struct __attribute__((packed)) {
    uint32_t counter;
    uint32_t time;
    float gx;
    float gy;
    float gz;
    float ax;
    float ay;
    float az;
} message_t;

typedef struct {
    uart_t *debug_uart;
    spi_t *flash_spi;
    w25q128jv_t flash;
    uint32_t current_page;
    uint32_t counter;
    ring_buffer_t ring_buffer;
} logger_t;

void logger_init(logger_t *logger, spi_t *spi, uart_t *debug_uart);
void logger_write(logger_t *logger, ins_t *ins);
void logger_erase(logger_t *logger, uint16_t sector);
void logger_read(logger_t *logger, uint32_t page, message_t *messages);
void logger_read_output(logger_t *logger);
void logger_erase_output(logger_t *logger);

#endif // LOGGER_H
