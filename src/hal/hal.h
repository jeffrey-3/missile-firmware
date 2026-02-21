#ifndef HAL_H
#define HAL_H

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "registers.h"

#define FREQ 16000000 // 16 MHz
#define BIT(x) (1UL << (x))

void _systick_handler(void);

void systick_init();
void delay(uint32_t duration);
uint32_t get_time();
bool timer_expired(uint32_t *expiration, uint32_t period);
void spin(volatile uint32_t count);

#endif // HAL_H
