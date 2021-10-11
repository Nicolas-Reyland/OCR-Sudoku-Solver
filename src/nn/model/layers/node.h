// node.h

#ifndef NN_NODE_H
#define NN_NODE_H

#include <stdlib.h>
#include <time.h>
#include "nn/utils/misc/randomness.h"

typedef struct nn_Node {
  unsigned int num_weights;
  double* weights;
  double bias;
  double raw_value;
  double value;
} nn_Node;

nn_Node* _nn_createNode(unsigned int num_weights);

#endif
