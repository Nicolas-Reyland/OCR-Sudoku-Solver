// struct_description.h

#ifndef NN_STRUCT_DESCRIPTION_H
#define NN_STRUCT_DESCRIPTION_H

#include <stddef.h>

typedef struct nn_ShapeDescription {
    size_t dims;
    size_t x;
    size_t y;
    size_t z;
    size_t range; // x * y * z
} nn_ShapeDescription;

nn_ShapeDescription nn_createShapeDescription(size_t x, size_t y, size_t z);
nn_ShapeDescription nn_create1DShapeDescription(size_t x);
nn_ShapeDescription nn_create2DShapeDescription(size_t x, size_t y);
nn_ShapeDescription nn_emptyShapeDescription(void);

#endif
