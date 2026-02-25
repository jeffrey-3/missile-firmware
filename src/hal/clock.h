#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>
#include <stdbool.h>
#include "registers.h"

#define FREQ 16000000 // 16 MHz

void systick_init();
void _systick_handler();
void delay(uint32_t duration);
uint32_t get_time();
bool timer_expired(uint32_t *expiration, uint32_t period);
void spin(volatile uint32_t count);

#endif // CLOCK_H
