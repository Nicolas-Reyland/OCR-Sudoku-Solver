# include <stdlib.h>
# include <stdio.h>

typedef struct Matrix{
    size_t h;
    size_t w;
    size_t *values;
} Matrix;

Matrix* init_Matrix(size_t height, size_t width);

void matrix_setvalue(Matrix *M, size_t i, size_t j, size_t value);

size_t matrix_getvalue(Matrix *M, size_t i, size_t j);

Matrix* matrix_multiply(Matrix *M1, Matrix *M2);

Matrix* matrix_transpose(Matrix *M);

Matrix* matrix_invert(Matrix *M);
