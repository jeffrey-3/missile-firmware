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

#define LOGGER_RING_BUF_SIZE 1024 // Must be a power of 2
#define LOGGER_MSG_PER_PAGE 5
#define LOGGER_SECTOR_ERASE_TIME 450
#define LOGGER_WRITE_EN_TIME 5
#define LOGGER_NUM_SECTORS 10
#define LOGGER_NUM_PAGES 30

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
    spi_t *flash_spi;
    w25q128jv_t flash;
    uint32_t current_page;
    uint32_t counter;
    ring_buffer_t ring_buffer;
    uint32_t timer;
} logger_t;

void logger_init(logger_t *logger, spi_t *spi);
void logger_update(logger_t *logger, ins_t *ins);

#endif // LOGGER_H
