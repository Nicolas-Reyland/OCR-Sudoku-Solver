// node.c

#include "node.h"
#include "utils/macros/macros.h"

nn_Node* _nn_createNode(size_t num_weights)
{
  double* weights, * d_weights;
  double bias;
  // malloc weights
  if (num_weights) {
    weights = mem_calloc(num_weights, sizeof(double));
    d_weights = mem_calloc(num_weights, sizeof(double));
    for (size_t i = 0; i < num_weights; i++) {
      weights[i] = 0.0;//getNormalizedRandomDouble();
    }
    bias = getNormalizedRandomDouble();
  } else {
    weights = NULL;
    d_weights = NULL;
    bias = NAN;
  }
  // malloc struct
  nn_Node* node = mem_malloc(sizeof(nn_Node));
  node->num_weights = num_weights;
  // weights
  node->weights = weights;
  node->d_weights = d_weights;
  // bias
  node->bias = bias;
  node->d_bias = NAN;
  // delta value
  node->delta_value = NAN;
  // raw values
  node->raw_value = NAN;
  // values
  node->value = NAN;
  node->d_value = NAN;
  return node;
}

void _nn_freeNode(nn_Node* node)
{
  if (node->weights != NULL) {
    // explicitly set to NULL for the output payer
    mem_free(node->weights);
  }
  if (node->d_weights != NULL) {
    // explicitly set to NULL for the output payer
    mem_free(node->d_weights);
  }
  mem_free(node);
}

//
