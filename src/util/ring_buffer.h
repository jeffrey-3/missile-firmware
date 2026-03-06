#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

#define RING_BUFFER_SIZE 1024 // Must be power of 2

typedef struct {
    uint8_t buffer[RING_BUFFER_SIZE];
    uint32_t mask;
    uint32_t read_index;
    uint32_t write_index;
} rb_u8_t;

typedef struct {
    float buffer[RING_BUFFER_SIZE];
    uint32_t mask;
    uint32_t read_index;
    uint32_t write_index;
} rb_f32_t;

void rb_setup_u8(rb_u8_t* rb);
void rb_setup_f32(rb_f32_t* rb);

uint32_t rb_write_u8(rb_u8_t* rb, uint8_t *data, uint32_t len);
uint32_t rb_write_f32(rb_f32_t* rb, float *data, uint32_t len);

uint32_t rb_read_u8(rb_u8_t* rb, uint8_t *data, uint32_t len);
uint32_t rb_read_f32(rb_f32_t* rb, float *data, uint32_t len);

uint32_t rb_count_u8(rb_u8_t* rb);
uint32_t rb_count_f32(rb_f32_t* rb);

#endif // RING_BUFFER_H
