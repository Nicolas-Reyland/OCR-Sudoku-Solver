// node.h

#ifndef NN_NODE_H
#define NN_NODE_H

#include "nn/utils/misc/randomness.h"
#include "utils/macros/macros.h"
#include "utils/mem/mem-management.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

typedef struct nn_Node nn_Node;
struct nn_Node {
    // weights
    size_t num_weights;
    double *weights;
    double *d_weights;
    // bias
    double bias;
    double d_bias;
    // values
    double value;
    double d_value;
    // raw values
    double raw_value;
    double d_raw_value;
};

nn_Node *_nn_createNode(size_t num_weights, bool init_bias);
void _nn_freeNode(nn_Node *node);

#endif
