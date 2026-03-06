#include <string.h>
#include <stdio.h>
#include "../../src/hal/clock.h"
#include "../../src/hal/uart.h"
#include "../../src/hal/spi.h"
#include "../../src/pins.h"

void test_led_blink();
void test_servo();
void test_calibrate();
void test_erase_flash();
void test_read_flash();

typedef struct {
    char key;
    const char *name;
    void (*func)(void);
} test_entry_t;

static const test_entry_t test_entries[] = {
    {'1', "Blink LED", test_led_blink},
    {'2', "Servo Test", test_servo},
    {'3', "Calibrate IMU", test_calibrate},
    {'*', "Erase Flash", test_erase_flash},
    {'#', "Read Flash", test_read_flash}
};

static const int num_tests = sizeof(test_entries) / sizeof(test_entries[0]);

int main(void) {
    systick_init();

    uart_t uart;
    uart_init(&uart, UART1, &pins.uart1_tx, &pins.uart1_rx, 115200);

    // Print menu
    for (uint8_t i = 0; i < num_tests; i++) {
        char uart_buf[200];
        snprintf(uart_buf, sizeof(uart_buf), "%c - %s\r\n", test_entries[i].key,
            test_entries[i].name);
        uart_write(&uart, uart_buf, strlen(uart_buf));
    }

    uint8_t rx_byte;

    // Store first character as key
    while (uart_empty(&uart));
    uart_read(&uart, &rx_byte);
    char key = (char)rx_byte;

    // Check if second character is ending
    while (uart_empty(&uart));
    uart_read(&uart, &rx_byte);
    char end = (char)rx_byte;

    if (end == '\n' || end == '\r') {
        // Check if key is found in test entries
        for (uint8_t i = 0; i < num_tests; i++) {
            if (test_entries[i].key == key) {
                test_entries[i].func(); // Execute test function
            }
        }
    }

    return 0;
}
