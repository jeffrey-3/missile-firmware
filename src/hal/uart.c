#include "uart.h"

static ring_buffer_t uart1_ring_buffer;

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
        NVIC->ISER |= (1 << 27); // Enable interupt
    }

    uart->uart_reg->CR1 = 0; // Disable this UART
    uart->uart_reg->BRR = FREQ / uart->baud; // FREQ is a UART bus frequency
    uart->uart_reg->CR1 |= 1UL; // Set UE
    uart->uart_reg->CR1 |= 1UL << 2; // Set RE
    uart->uart_reg->CR1 |= 1UL << 3; // Set TE
    uart->uart_reg->CR1 |= 1UL << 5; // Enable interrupt

    ring_buffer_setup(&uart1_ring_buffer);
}

void uart_write(uart_t *uart, char *buf, size_t len) {
    while (len-- > 0) {
        uart->uart_reg->TDR = *(uint8_t *)buf++;

        // Wait until transmit complete (TXE bit set) before sending next
        while ((uart->uart_reg->ISR & (1UL << 7)) == 0) spin(1);
    }
}

void uart_read(uart_t *uart, uint8_t* byte) {
    if (uart->uart_reg == UART1) {
        uint8_t data[1];
        ring_buffer_read(&uart1_ring_buffer, data, 1);
        *byte = data[0];
    }
}

bool uart_empty(uart_t *uart) {
    if (uart->uart_reg == UART1) {
        return ring_buffer_count(&uart1_ring_buffer) == 0;
    }

    return true;
}

void _uart1_irq_handler() {
    uint8_t rx_byte[1] = {(uint8_t)(UART1->RDR & 255)};
    ring_buffer_write(&uart1_ring_buffer, rx_byte, 1);
}
