#ifndef MATH_H
#define MATH_H

#include <stdint.h>

#define DEG2RAD (3.1415927f / 180.0f)
#define RAD2DEG (180.0f / 3.1415927f)

uint16_t clamp_u16(uint16_t value, uint16_t min, uint16_t max);

#endif // MATH_H
