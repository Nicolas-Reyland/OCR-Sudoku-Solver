// sample.c

#include "sample.h"

void _nn_print(nn_Sample* sample)
{
  for(size_t i = 0; i < sample->nb_values;i++)
    printf("%f, ", sample->values[i]);
  printf("\n");
}

nn_Sample* createSample(nn_ShapeDescription description, double* values,
size_t nb_values)
{
  nn_Sample* sample         = mem_malloc(sizeof(nn_Sample));
  sample->shape_description = description;
  sample->nb_values         = nb_values;
  sample->values            = values;
  sample->print             = &_nn_print;


  return sample;
}

void freeSample(nn_Sample* sample)
{
  if(sample == NULL)
  {
      verbose("freeSample: sample is null.");
      exit(EXIT_FAILURE);
  }
  if(sample->values == NULL)
  {
      verbose("freeSample: values array is null.");
      exit(EXIT_FAILURE);
  }
  mem_free(sample->values);
  mem_free(sample);
}
