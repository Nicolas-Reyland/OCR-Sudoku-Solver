// node.h

#ifndef NN_NODE_H
#define NN_NODE_H

#include <stdlib.h>
#include <time.h>
#include "../../utils/misc/randomness.h"

typedef struct nn_Node {
  unsigned int num_weights;
  double* weights;
  double bias;
} nn_Node;

nn_Node _nn_initNode(unsigned int num_weights);

#endif
