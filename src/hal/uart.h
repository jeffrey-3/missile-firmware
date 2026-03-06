#ifndef UART_H
#define UART_H

#include <stddef.h>
#include "registers.h"
#include "clock.h"
#include "gpio.h"
#include "../util/ring_buffer.h"

typedef struct {
    struct uart_reg *uart_reg;
    gpio_t *tx;
    gpio_t *rx;
    unsigned long baud;
} uart_t;

void uart_init(uart_t *uart, struct uart_reg *uart_reg, gpio_t *tx, gpio_t *rx,
    unsigned long baud);
void uart_write(uart_t *uart, char *buf, size_t len);
void uart_read(uart_t *uart, uint8_t* byte);
bool uart_empty(uart_t *uart);
void _uart1_irq_handler();

#endif // UART_H
