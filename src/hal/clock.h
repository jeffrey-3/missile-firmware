#ifndef CLOCK_H
#define CLOCK_H

#define FREQ 16000000 // 16 MHz

void _systick_handler(void);

void systick_init();
void delay(uint32_t duration);
uint32_t get_time();
bool timer_expired(uint32_t *expiration, uint32_t period);
void spin(volatile uint32_t count);

#endif // CLOCK_H
