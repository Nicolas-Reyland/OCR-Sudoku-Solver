// sample.h

#ifndef NN_SAMPLE_H
#define NN_SAMPLE_H

#include "nn/utils/structs/shape_description.h"
#include <stdlib.h>

typedef struct nn_Sample
{
    nn_ShapeDescription shape_description;
    double* values;
} nn_Sample;

nn_Sample* createSample(nn_ShapeDescription description, double* values);
void freeSample(nn_Sample* sample);

#endif
