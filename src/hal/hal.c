#include "hal.h"

static volatile uint32_t current_time = 0;

void _systick_handler(void) {
    current_time++;
}

void systick_init() {
    SYSTICK->LOAD = FREQ / 1000 - 1; // Tick every 1ms
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2); // Enable systick
    RCC->APBENR2 |= BIT(0); // Enable SYSCFG
}

void delay(uint32_t duration) {
    uint32_t start = current_time;
    while (current_time - start < duration) spin(1);
}

uint32_t get_time() {
    return current_time;
}

bool timer_expired(uint32_t *expiration, uint32_t period) {
    // Reset expiration if time wrapped
    if (current_time + period < *expiration) {
        *expiration = 0;
    }

    // Check if first poll and set expiration
    if (*expiration == 0) {
        *expiration = current_time + period;
    }

    // Check if timer expired
    if (*expiration > current_time) {
        return false; // Not expired, return false
    } else {
        // Set next expiration time
        if (current_time - *expiration > period) {
            // If you missed a period, set expiration relative to now
            *expiration = current_time + period;
        } else {
            *expiration += period;
        }
        return true; // Expired, return true
    }

}

void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

