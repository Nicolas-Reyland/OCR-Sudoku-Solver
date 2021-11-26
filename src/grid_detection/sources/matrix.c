#include "../headers/matrix.h"

Matrix* init_Matrix(size_t height, size_t width)
{
    Matrix *M = malloc(sizeof(Matrix));
    M->h = height;
    M->w = width;
    M->values = calloc(height*width ,sizeof(size_t));

    return M;
}

void matrix_setvalue(Matrix *M, size_t i, size_t j, size_t value)
{
    if (i > M->h || j > M->w)
    {
        printf("setvalue: wrong index i or j");
        return;
    }
    M->values[i*M->w + j] = value;
}

size_t matrix_getvalue(Matrix *M, size_t i, size_t j)
{
    if (i > M->h || j > M->w)
    {
        printf("getvalue: wrong index i or j");
        return 0;
    }
    return M->values[i*M->w + j];
}

Matrix* matrix_multiply(Matrix *M1, Matrix *M2)
{
    if (M1->w != M2->h || M1->h != M2->w)
    {
        printf("matrix_multiply: wrong size");
        return init_Matrix(0,0);
    }
    Matrix *new = init_Matrix(M1->h, M2->w);
    for (size_t i = 0; i < M1->h; i++)
    {
        for (size_t j = 0; j < M1->w; j++)
        {
            matrix_setvalue(new, i, j, matrix_getvalue(M1, i, j) * matrix_getvalue(M2, j, i));
        }
    }
    return new;
}

Matrix* matrix_transpose(Matrix *M)
{
    Matrix *new = init_Matrix(M->w, M->h);
    for (size_t i = 0; i < M->h; i++)
    {
        for (size_t j = 0; j < M->w; j++)
        {
            matrix_setvalue(new, j, i, matrix_getvalue(M, i, j));
        }
    }
    return new;
}

size_t matrix_determinant(Matrix *M)
{
    if (M->h != M->w)
    {
        printf("matrix_determinant: not squared matrix");
        return 0;
    }
    size_t size = M->w;
    size_t D = 0;

    if (size == 1)
    {
        return matrix_getvalue(M, 0, 0);
    }

    int sign = 1;
    Matrix *C = init_Matrix(size, size);

    for (size_t j = 0; j < M->w; j++)
    {
        matrix_cofactor(M, C, 0, j, size);
        D += sign * matrix_getvalue(M, 0, j) * matrix_determinant(C);
    }

    return D;
}

Matrix* matrix_invert(Matrix *M)
{
    if (M->h != M->w)
    {
        printf("matrix_invert: not squared matrix");
        return init_Matrix(0, 0);
    }

}
