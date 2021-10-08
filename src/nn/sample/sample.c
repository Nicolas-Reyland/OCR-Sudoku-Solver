#include "sample.h"
#include <stdio.h>

nn_Sample* createSample(nn_ShapeDescription description, double* values)
{
  nn_Sample* sample         = malloc(sizeof(nn_Sample));
  sample->shape_description = description;
  sample->values            = values;

  return sample;
}

void freeSample(nn_Sample* sample)
{
  free(sample->values);
  free(sample);
}
