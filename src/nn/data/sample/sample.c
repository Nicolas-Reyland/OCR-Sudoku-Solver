// sample.c

#include "sample.h"

nn_Sample* createSample(nn_ShapeDescription description, double* values)
{
  nn_Sample* sample         = mem_malloc(sizeof(nn_Sample));
  sample->shape_description = description;
  sample->values            = values;

  return sample;
}

void freeSample(nn_Sample* sample)
{
  mem_free(sample->values);
  mem_free(sample);
}
