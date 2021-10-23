// dense.h

#ifndef NN_DENSE_H
#define NN_DENSE_H

#include "node/node.h"
#include "nn/functions_descriptors/functions_descriptors_enums.h"

typedef struct nn_LayerDense nn_LayerDense;
struct nn_LayerDense {
  nn_ShapeDescription shape;
  activation activation;
  size_t num_nodes;
  nn_Node** nodes;
};

struct _nn_LayerCreationArguments_Dense {
  nn_ShapeDescription layer_shape;
  nn_ShapeDescription next_layer_shape;
  activation activation;
  bool alloc_weights;
  bool init_bias;
};

nn_LayerDense* _nn_createLayerDense(struct _nn_LayerCreationArguments_Dense* args);
void _nn_freeLayerDense(nn_LayerDense* layer);

#endif
