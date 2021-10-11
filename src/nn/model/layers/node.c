// node.c

#include "node.h"

nn_Node* _nn_createNode(unsigned int num_weights)
{
  // malloc weights
  double *weights = calloc(num_weights, sizeof(double));
  for (unsigned int i = 0; i < num_weights; i++) {
    weights[i] = 0.0;//getNormalizedRandomDouble();
  }
  double bias = getNormalizedRandomDouble();
  // malloc struct
  nn_Node* node = malloc(sizeof(nn_Node));
  node->num_weights = num_weights;
  node->weights = weights;
  node->bias = bias;
  return node;
}


//
