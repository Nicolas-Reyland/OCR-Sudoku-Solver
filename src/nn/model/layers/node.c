// node.c

#include "node.h"
#include <stdlib.h>

nn_Node _nn_initNode(unsigned int num_weights)
{
  double* weights = malloc(num_weights * sizeof(double));
  for (unsigned int i = 0; i < num_weights; i++) {
    *(weights + i) = getNormalizedRandomDouble();
  }
  double bias = getNormalizedRandomDouble();
  nn_Node node = {
    .num_weights = num_weights,
    .weights = weights,
    .bias = bias
  };
  return node;
}


//
