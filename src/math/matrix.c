#include "matrix.h"
#include <stdio.h>

void mat_zero(matrix_t *m, uint8_t rows, uint8_t cols) {
    m->rows = rows;
    m->cols = cols;

    for (uint8_t i = 0; i < rows; i++)
        for (uint8_t j = 0; j < cols; j++)
            m->data[i][j] = 0.0f;
}

void mat_identity(matrix_t *m, uint8_t size) {
    mat_zero(m, size, size);

    for (uint8_t i = 0; i < size; i++)
        m->data[i][i] = 1.0f;
}

void mat_copy(matrix_t *dst, const matrix_t *src) {
    dst->rows = src->rows;
    dst->cols = src->cols;

    for (uint8_t i = 0; i < src->rows; i++)
        for (uint8_t j = 0; j < src->cols; j++)
            dst->data[i][j] = src->data[i][j];
}

bool mat_add(matrix_t *result, const matrix_t *a, const matrix_t *b) {
    if (a->rows != b->rows || a->cols != b->cols)
        return false;

    result->rows = a->rows;
    result->cols = a->cols;

    for (uint8_t i = 0; i < a->rows; i++)
        for (uint8_t j = 0; j < a->cols; j++)
            result->data[i][j] = a->data[i][j] + b->data[i][j];

    return true;
}

bool mat_sub(matrix_t *result, const matrix_t *a, const matrix_t *b) {
    if (a->rows != b->rows || a->cols != b->cols)
        return false;

    result->rows = a->rows;
    result->cols = a->cols;

    for (uint8_t i = 0; i < a->rows; i++)
        for (uint8_t j = 0; j < a->cols; j++)
            result->data[i][j] = a->data[i][j] - b->data[i][j];

    return true;
}

bool mat_mul(matrix_t *result, const matrix_t *a, const matrix_t *b) {
    if (a->cols != b->rows)
        return false;

    result->rows = a->rows;
    result->cols = b->cols;

    for (uint8_t i = 0; i < result->rows; i++) {
        for (uint8_t j = 0; j < result->cols; j++) {
            float sum = 0.0f;

            for (uint8_t k = 0; k < a->cols; k++)
                sum += a->data[i][k] * b->data[k][j];

            result->data[i][j] = sum;
        }
    }

    return true;
}

bool mat_transpose(matrix_t *result, const matrix_t *m) {
    if (m->rows > MAT_MAX_COLS || m->cols > MAT_MAX_ROWS)
        return false;

    result->rows = m->cols;
    result->cols = m->rows;

    for (uint8_t i = 0; i < m->rows; i++)
        for (uint8_t j = 0; j < m->cols; j++)
            result->data[j][i] = m->data[i][j];

    return true;
}
