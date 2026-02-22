#include "uart.h"

void uart_init(uart_t *uart, struct uart_reg *uart_reg, gpio_t *tx, gpio_t *rx,
    unsigned long baud) {
    uart->uart_reg = uart_reg;
    uart->tx = tx;
    uart->rx = rx;
    uart->baud = baud;

    gpio_init(uart->tx);
    gpio_init(uart->rx);

    if (uart->uart_reg == UART1) {
        RCC->APBENR2 |= 1UL << 14;
    }

    uart->uart_reg->CR1 = 0; // Disable this UART
    uart->uart_reg->BRR = FREQ / uart->baud; // FREQ is a UART bus frequency
    uart->uart_reg->CR1 |= 1UL | (1UL << 2) | (1UL << 3); // Set UE, RE, TE
}

void uart_write_byte(uart_t *uart, uint8_t byte) {
    uart->uart_reg->TDR = byte;

    // Finished transmit when TXE is set
    while ((uart->uart_reg->ISR & (1UL << 7)) == 0) spin(1);
}

void uart_write_buf(uart_t *uart, char *buf, size_t len) {
    while (len-- > 0) {
        uart_write_byte(uart, *(uint8_t *)buf++);
    }
}

int uart_read_ready(uart_t *uart) {
    return uart->uart_reg->ISR & (1UL << 5); // If RXNE bit is set, data is ready
}

uint8_t uart_read_byte(uart_t *uart) {
    return (uint8_t)(uart->uart_reg->RDR & 255);
}

