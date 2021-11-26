#include "../headers/homography.h"

Tuple* findhomography(Tuple src, size_t size)
{
    size_t x1 = src.values[0];
    size_t y1 = src.values[1];
    size_t x2 = src.values[2];
    size_t y2 = src.values[3];
    size_t x3 = src.values[4];
    size_t y3 = src.values[5];
    size_t x4 = src.values[6];
    size_t y4 = src.values[7];

    Matrix *A = init_Matrix(8, 8);

    matrix_setvalue(A, 0, 2, 1);
    matrix_setvalue(A, 1, 5, 1);
    matrix_setvalue(A, 2, 0, size);
    matrix_setvalue(A, 2, 2, 1);
    matrix_setvalue(A, 2, 6, -size * x2);
    matrix_setvalue(A, 3, 3, size);
    matrix_setvalue(A, 3, 5, 1);
    matrix_setvalue(A, 3, 6, -size * y2);
    matrix_setvalue(A, 4, 1, size);
    matrix_setvalue(A, 4, 2, 1);
    matrix_setvalue(A, 4, 7, -size * x3);
    matrix_setvalue(A, 5, 4, size);
    matrix_setvalue(A, 5, 5, 1);
    matrix_setvalue(A, 5, 7, -size * y3);
    matrix_setvalue(A, 6, 0, size);
    matrix_setvalue(A, 6, 1, size);
    matrix_setvalue(A, 6, 2, 1);
    matrix_setvalue(A, 6, 6, -size * x4);
    matrix_setvalue(A, 6, 7, -size * x4);
    matrix_setvalue(A, 7, 3, size);
    matrix_setvalue(A, 7, 4, size);
    matrix_setvalue(A, 7, 5, 1);
    matrix_setvalue(A, 7, 6, -size * y4);
    matrix_setvalue(A, 7, 7, -size * y4);

    Matrix *B = init_Matrix(8, 1);

    matrix_setvalue(B, 0, 0, x1);
    matrix_setvalue(B, 1, 0, y1);
    matrix_setvalue(B, 2, 0, x2);
    matrix_setvalue(B, 3, 0, y2);
    matrix_setvalue(B, 4, 0, x3);
    matrix_setvalue(B, 5, 0, y3);
    matrix_setvalue(B, 6, 0, x4);
    matrix_setvalue(B, 7, 0, y4);

    Matrix *A_t = matrix_transpose(A);

    Matrix *h = matrix_multiply(matrix_multiply(
    matrix_invert(matrix_multiply(A_t, A)), A_t), B);

    size_t *val = malloc(sizeof(size_t)*8);

    val[0] = matrix_getvalue(h, 0, 0);
    val[1] = matrix_getvalue(h, 0, 1);
    val[2] = matrix_getvalue(h, 0, 2);
    val[3] = matrix_getvalue(h, 0, 3);
    val[4] = matrix_getvalue(h, 0, 4);
    val[5] = matrix_getvalue(h, 0, 5);
    val[6] = matrix_getvalue(h, 0, 6);
    val[7] = matrix_getvalue(h, 0, 7);

    Tuple *tuple = createTuple(val ,8);
    return tuple;
}

SDL_Surface* transformimage(Tuple *transform, SDL_Surface *image, size_t size)
{
    size_t a = transform->values[0];
    size_t b = transform->values[1];
    size_t c = transform->values[2];
    size_t d = transform->values[3];
    size_t e = transform->values[4];
    size_t f = transform->values[5];
    size_t g = transform->values[6];
    size_t h = transform->values[7];

    SDL_Surface *output = SDL_CreateRGBSurface(0, size, size, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);

    for (size_t y = 0; y < (size_t)image->h; y++)
    {
      size_t sxPre1 = b * y + c;
      size_t sxPre2 = h * y + 1;
      size_t syPre1 = e * y + f;
      size_t syPre2 = h * y + 1;

      for (size_t x = 0; x < (size_t)image->w; x++)
      {
        size_t sx = floor((a * x + sxPre1)/(g * x + sxPre2));
        size_t sy = floor((d * x + syPre1)/(g * x + syPre2));

        Uint32 pix = get_pixel(image, sx, sy);
        Uint8 color;
        SDL_GetRGB(pix, image->format, &color, &color, &color);

        put_pixel(output, x, y, SDL_MapRGB(output->format, color, color, color));
      }
    }
    return output;
}
