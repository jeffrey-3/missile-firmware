#include "ring_buffer.h"

void ring_buffer_setup(ring_buffer_t* rb) {
    rb->read_index = 0;
    rb->write_index = 0;
    rb->mask = RING_BUFFER_SIZE - 1;
}

uint32_t ring_buffer_read(ring_buffer_t* rb, uint8_t* data, uint32_t len) {
    uint32_t count = 0;

    while (count < len) {
        uint32_t local_read_index = rb->read_index;
        uint32_t local_write_index = rb->write_index;

        if (local_read_index == local_write_index) {
            break; // Buffer is empty
        }

        data[count++] = rb->buffer[local_read_index];
        local_read_index = (local_read_index + 1) & rb->mask;
        rb->read_index = local_read_index;
    }

    return count;
}

uint32_t ring_buffer_write(ring_buffer_t* rb, uint8_t *data, uint32_t len) {
    uint32_t count = 0;

    while (count < len) {
        uint32_t local_write_index = rb->write_index;
        uint32_t local_read_index = rb->read_index;

        uint32_t next_write_index = (local_write_index + 1) & rb->mask;

        if (next_write_index == local_read_index) {
            break; // Buffer is full
        }

        rb->buffer[local_write_index] = data[count++];
        rb->write_index = next_write_index;
    }

    return count;
}

uint32_t ring_buffer_count(ring_buffer_t* rb) {
    return (rb->write_index - rb->read_index) & rb->mask;
}
