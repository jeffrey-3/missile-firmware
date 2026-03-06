#include "ring_buffer.h"

void ring_buffer_setup(ring_buffer_t* rb) {
    rb->read_index = 0;
    rb->write_index = 0;
    rb->mask = RING_BUFFER_SIZE - 1;
}

bool ring_buffer_empty(ring_buffer_t* rb) {
    return rb->read_index == rb->write_index;
}

bool ring_buffer_read(ring_buffer_t* rb, uint8_t* byte) {
    uint32_t local_read_index = rb->read_index;
    uint32_t local_write_index = rb->write_index;

    if (local_read_index == local_write_index) {
        return false;
    }

    *byte = rb->buffer[local_read_index];
    local_read_index = (local_read_index + 1) & rb->mask;
    rb->read_index = local_read_index;

    return true;
}

void ring_buffer_read_arr(ring_buffer_t* rb, uint8_t* arr, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        ring_buffer_read(rb, &arr[i]);
    }
}

bool ring_buffer_write(ring_buffer_t* rb, uint8_t byte) {
    uint32_t local_write_index = rb->write_index;
    uint32_t local_read_index = rb->read_index;

    uint32_t next_write_index = (local_write_index + 1) & rb->mask;

    if (next_write_index == local_read_index) {
        return false;
    }

    rb->buffer[local_write_index] = byte;
    rb->write_index = next_write_index;
    return true;
}

void ring_buffer_write_arr(ring_buffer_t* rb, uint8_t *arr, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        ring_buffer_write(rb, arr[i]);
    }
}

uint32_t ring_buffer_count(ring_buffer_t* rb) {
    return (rb->write_index - rb->read_index) & rb->mask;
}
