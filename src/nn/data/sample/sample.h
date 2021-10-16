// sample.h

#ifndef NN_SAMPLE_H
#define NN_SAMPLE_H

#include <stdlib.h>
#include "nn/utils/structs/shape_description.h"
#include "utils/mem/mem-management.h"

typedef struct nn_Sample
{
    nn_ShapeDescription shape_description;
    size_t nb_values;
    double* values;
} nn_Sample;

nn_Sample* createSample(nn_ShapeDescription description, double* values,
size_t nb_values);
void freeSample(nn_Sample* sample);

#endif
