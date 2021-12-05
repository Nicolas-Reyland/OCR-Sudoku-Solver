// shape_description.c

#include "shape_description.h"

nn_ShapeDescription nn_createShapeDescription(size_t x, size_t y, size_t z) {
    nn_ShapeDescription shape = {
        .dims = (x != 1) + (y != 1) + (z != 1),
        .x = x,
        .y = y,
        .z = z,
        .range = x * y * z,
    };
    return shape;
}

nn_ShapeDescription nn_create1DShapeDescription(size_t x) {
    return nn_createShapeDescription(x, 1, 1);
}

nn_ShapeDescription nn_create2DShapeDescription(size_t x, size_t y) {
    return nn_createShapeDescription(x, y, 1);
}

nn_ShapeDescription nn_emptyShapeDescription(void) {
    return nn_createShapeDescription(0, 0, 0);
}
