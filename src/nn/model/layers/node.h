// node.h

#ifndef NN_NODE_H
#define NN_NODE_H

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "utils/macros/macros.h"
#include "nn/utils/misc/randomness.h"
#include "utils/mem/mem-management.h"

typedef struct nn_Node {
  size_t num_weights;
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

nn_Node* _nn_createNode(size_t num_weights, bool init_bias);
void _nn_freeNode(nn_Node* node);

#endif
