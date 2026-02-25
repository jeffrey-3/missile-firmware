#include <string.h>
#include <stdio.h>
#include "../../src/hal/pwm.h"
#include "../../src/hal/clock.h"
#include "../../src/hal/uart.h"
#include "../../src/hal/spi.h"
#include "../../src/logger.h"
#include "../../src/board.h"

static uint8_t data_buffer[LOGGER_RING_BUF_SIZE] = {0};

void test_led_blink(void) {
    gpio_init(&board_pins.led);

    bool led_on = false;

    for (;;) {
        gpio_write(&board_pins.led, led_on);
        led_on = !led_on;
        delay(200);
    }
}

void test_servo(void) {
    gpio_init(&board_pins.led);

    pwm_t servo_y;
    pwm_t servo_z;
    pwm_init(&servo_y, TIM3, &board_pins.tim3_ch2, 2, 333.0f);
    pwm_init(&servo_z, TIM1, &board_pins.tim1_ch4, 4, 333.0f);

    bool led_on = false;

    for (;;) {
        gpio_write(&board_pins.led, led_on);
        led_on = !led_on;

        if (led_on) {
            pwm_set_pulse(&servo_y, 2100);
            delay(200);
            pwm_set_pulse(&servo_y, 900);
            delay(200);
            pwm_set_pulse(&servo_y, 1500);
            delay(200);
        } else {
            pwm_set_pulse(&servo_z, 2100);
            delay(200);
            pwm_set_pulse(&servo_z, 900);
            delay(200);
            pwm_set_pulse(&servo_z, 1500);
            delay(200);
        }
    }
}

void test_calibrate(void) {
    return;
}

void test_erase_flash(void) {
    spi_t spi;
    uart_t uart;
    w25q128jv_t flash;

    uart_init(&uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx, 115200);
    spi_init(&spi, SPI2, &board_pins.spi2_cs, &board_pins.spi2_miso,
        &board_pins.spi2_mosi, &board_pins.spi2_sck);
    w25q128jv_init(&flash, &spi);

    // Erase every sector one by one
    for (uint16_t i = 0; i < LOGGER_NUM_SECTORS; i++) {
        // After every erase, the flash chip disables write, so must re-enable
        w25q128jv_write_enable(&flash);
        delay(5);

        w25q128jv_erase_sector(&flash, i);
        delay(500);

        char uart_buf[100];
        snprintf(uart_buf, sizeof(uart_buf), "Erased %d out of %d\r\n",
            i + 1, LOGGER_NUM_SECTORS);
        uart_write_buf(&uart, uart_buf, strlen(uart_buf));
    }

    char uart_buf[100] = "Finished\r\n";
    uart_write_buf(&uart, uart_buf, strlen(uart_buf));
}

void test_read_flash(void) {
    spi_t spi;
    uart_t uart;
    w25q128jv_t flash;
    ring_buffer_t ring_buffer;

    ring_buffer_setup(&ring_buffer, data_buffer, LOGGER_RING_BUF_SIZE);
    uart_init(&uart, UART1, &board_pins.uart1_tx, &board_pins.uart1_rx, 115200);
    spi_init(&spi, SPI2, &board_pins.spi2_cs, &board_pins.spi2_miso,
        &board_pins.spi2_mosi, &board_pins.spi2_sck);
    w25q128jv_init(&flash, &spi);

    // Read each page one by one
    for (uint32_t i = 0; i < LOGGER_NUM_PAGES; i++) {
        char buf[100];
        snprintf(buf, sizeof(buf), "Reading page %ld out of %d\r\n", i + 1,
            LOGGER_NUM_PAGES);
        uart_write_buf(&uart, buf, strlen(buf));

        // Read this page and get array of message structs
        w25q128jv_write_disable(&flash);
        delay(5);

        uint8_t data[LOGGER_PAGE_SIZE];
        w25q128jv_read(&flash, i, 0, LOGGER_PAGE_SIZE, data);
        ring_buffer_write_arr(&ring_buffer, data, LOGGER_PAGE_SIZE);

        while (ring_buffer_count(&ring_buffer) > sizeof(message_t)) {
            uint8_t message_bytes[sizeof(message_t)];
            ring_buffer_read_arr(&ring_buffer, message_bytes,
                sizeof(message_t));

            message_t message;
            memcpy(&message, message_bytes, sizeof(message_t));

            // Print contents of message
            char uart_buf[100];
            snprintf(uart_buf, sizeof(uart_buf),
                "%ld,%ld,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\r\n",
                message.counter, message.time, (double)message.gx,
                (double)message.gy, (double)message.gz, (double)message.ax,
                (double)message.ay, (double)message.az);
            uart_write_buf(&uart, uart_buf, strlen(uart_buf));
        }
    }

    uart_write_buf(&uart, "Finished\r\n", strlen("Finished\r\n"));
}

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
