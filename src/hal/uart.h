#ifndef UART_H
#define UART_H

#include <stddef.h>
#include "registers.h"
#include "clock.h"
#include "gpio.h"
#include "../util/ring_buffer.h"

#define UART_RING_BUF_SIZE 1024 // Must be a power of 2

typedef struct {
    struct uart_reg *uart_reg;
    gpio_t *tx;
    gpio_t *rx;
    unsigned long baud;
} uart_t;

void uart_init(uart_t *uart, struct uart_reg *uart_reg, gpio_t *tx, gpio_t *rx,
    unsigned long baud);
void uart_write_byte(uart_t *uart, uint8_t byte);
void uart_write_buf(uart_t *uart, char *buf, size_t len);
int uart_read_ready(uart_t *uart);
uint8_t uart_read_byte(uart_t *uart);
bool uart_ring_buffer_empty();
void uart_ring_buffer_read(uint8_t* byte);
void _uart1_irq_handler();

#endif // UART_H
