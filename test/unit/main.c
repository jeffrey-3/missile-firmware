#include "../../src/math/matrix.h"

int main(void) {
    matrix_t A, B, C;

    mat_zero(&A, 2, 2);
    mat_zero(&B, 2, 2);

    A.data[0][0] = 1; A.data[0][1] = 2;
    A.data[1][0] = 3; A.data[1][1] = 4;

    B.data[0][0] = 5; B.data[0][1] = 6;
    B.data[1][0] = 7; B.data[1][1] = 8;

    mat_mul(&C, &A, &B);

    return 0;
}
