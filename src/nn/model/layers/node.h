// node.h

#ifndef NN_NODE_H
#define NN_NODE_H

#include <stdlib.h>
#include <time.h>
#include "nn/utils/misc/randomness.h"
#include "utils/mem/mem-management.h"

typedef struct nn_Node {
  unsigned int num_weights;
  // weights
  double* weights;
  double* d_weights;
  // bias
  double bias;
  double d_bias;
  // raw values
  double raw_value;
  // delta value
  double delta_value;
  // values
  double value;
  double d_value;
} nn_Node;

nn_Node* _nn_createNode(unsigned int num_weights);
void _nn_freeNode(nn_Node* node);

#endif
