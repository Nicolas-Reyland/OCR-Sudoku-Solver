// node.c

#include "node.h"

nn_Node* _nn_createNode(unsigned int num_weights)
{
  double* weights;
  double bias;
  // malloc weights
  if (num_weights) {
    weights = mem_calloc(num_weights, sizeof(double));
    for (unsigned int i = 0; i < num_weights; i++) {
      weights[i] = 0.0;//getNormalizedRandomDouble();
    }
    bias = getNormalizedRandomDouble();
  } else {
    weights = NULL;
    bias = 0;
  }
  // malloc struct
  nn_Node* node = mem_malloc(sizeof(nn_Node));
  node->num_weights = num_weights;
  node->weights = weights;
  node->bias = bias;
  node->raw_value = 0.0/0.0; // NAN (Not A Number)
  node->value = 0.0/0.0; // NAN
  return node;
}

void _nn_freeNode(nn_Node* node)
{
  if (node->weights != NULL) {
    // explicitly set to NULL for the output payer
    mem_free(node->weights);
  }
  mem_free(node);
}

//
