#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>
#include <stdbool.h>

#define MAT_MAX_ROWS 6
#define MAT_MAX_COLS 6

typedef struct {
    uint8_t rows;
    uint8_t cols;
    float data[MAT_MAX_ROWS][MAT_MAX_COLS];
} matrix_t;

void mat_zero(matrix_t *m, uint8_t rows, uint8_t cols);
void mat_identity(matrix_t *m, uint8_t size);
void mat_copy(matrix_t *dst, const matrix_t *src);

bool mat_add(matrix_t *result, const matrix_t *a, const matrix_t *b);
bool mat_sub(matrix_t *result, const matrix_t *a, const matrix_t *b);
bool mat_mul(matrix_t *result, const matrix_t *a, const matrix_t *b);
bool mat_transpose(matrix_t *result, const matrix_t *m);

// void mat_print(const matrix_t *m);

#endif // MATRIX_H
