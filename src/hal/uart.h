#ifndef UART_H
#define UART_H

#include <stddef.h>
#include "registers.h"
#include "gpio.h"

typedef struct {
    struct uart_reg *uart_reg;
    gpio_t tx;
    gpio_t rx;
    unsigned long baud;
} uart_t;

void uart_init(uart_t *uart);
void uart_write_byte(uart_t *uart, uint8_t byte);
void uart_write_buf(uart_t *uart, char *buf, size_t len);
int uart_read_ready(uart_t *uart);
uint8_t uart_read_byte(uart_t *uart);

#endif // UART_H
