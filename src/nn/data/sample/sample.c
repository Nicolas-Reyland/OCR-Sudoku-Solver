// sample.c

#include "sample.h"

static void _nn_printSample(nn_Sample sample) {
    for (size_t i = 0; i < sample.num_values; i++)
        printf("%f, ", sample.values[i]);
    printf("\n");
}

nn_Sample createSample(nn_ShapeDescription description, double *values,
                       size_t num_values) {
    nn_Sample sample;
    sample.shape_description = description;
    sample.num_values = num_values;
    sample.values = values;
    sample.print = &_nn_printSample;
    return sample;
}

void freeSample(nn_Sample sample) { mem_free(sample.values); }
