#include <string.h>
#include <stdio.h>
#include "../../src/hal/timer.h"
#include "../../src/hal/clock.h"
#include "../../src/hal/uart.h"
#include "../../src/board.h"

typedef void (*test_func_t)(void);

typedef struct {
    char key;
    const char *name;
    test_func_t func;
} test_entry_t;

void test_led_blink(void) {
    gpio_init(&board_pins.led);

    bool led_on = false;
    uint32_t led_timer = 0;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            gpio_write(&board_pins.led, led_on);
            led_on = !led_on;
        }
    }
}

void test_servo(void) {
    gpio_init(&board_pins.led);

    timer_t servo_y;
    timer_t servo_z;
    timer_init(&servo_y, TIM3, &board_pins.tim3_ch2, 2);
    timer_init(&servo_z, TIM1, &board_pins.tim1_ch4, 4);

    bool led_on = false;
    uint32_t led_timer = 0;

    for (;;) {
        if (timer_expired(&led_timer, 500)) {
            gpio_write(&board_pins.led, led_on);
            led_on = !led_on;

            if (led_on) {
                timer_set_duty(&servo_y, 0.7f);
                timer_set_duty(&servo_z, 0.7f);
            } else {
                timer_set_duty(&servo_y, 0.3f);
                timer_set_duty(&servo_z, 0.3f);
            }
        }
    }
}

static const test_entry_t test_entries[] = {
    {'1', "Blink LED", test_led_blink},
    {'a', "Servo Test", test_servo}
};

static const int num_tests = sizeof(test_entries) / sizeof(test_entries[0]);

int main(void) {
    systick_init();

    uart_t uart;
    uart_init(&uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx, 115200);

    // Print menu
    for (uint8_t i = 0; i < num_tests; i++) {
        char uart_buf[200];
        snprintf(uart_buf, sizeof(uart_buf), "%c - %s\r\n", test_entries[i].key,
            test_entries[i].name);
        uart_write_buf(&uart, uart_buf, strlen(uart_buf));
    }

    // Store first character as key
    while (!uart_read_ready(&uart));
    char key = uart_read_byte(&uart);

    // Check if second character is ending
    while (!uart_read_ready(&uart));
    char end = uart_read_byte(&uart);
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
