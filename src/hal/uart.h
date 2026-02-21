#ifndef UART_H
#define UART_H

typedef struct {
    struct uart_reg *uart_reg;
    unsigned long baud;
} uart_t;

void uart_init(uart_t *uart);
void uart_write_byte(uart_t *uart, uint8_t byte);
void uart_write_buf(uart_t *uart, char *buf, size_t len);
int uart_read_ready(uart_t *uart);
uint8_t uart_read_byte(uart_t *uart);

#endif // UART_H
