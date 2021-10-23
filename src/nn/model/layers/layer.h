// layer.h

#ifndef NN_LAYER_H
#define NN_LAYER_H

#include <stdbool.h>
#include "nn/utils/structs/shape_description.h"
#include "nn/functions_descriptors/functions_descriptors_enums.h"
#include "dense.h"
#include "normalization.h"
#include "conv1D.h"
#include "conv2D.h"

typedef enum layerType layerType;
enum layerType {
  DENSE,
  NORMALIZATION,
  CONV1D,
  CONV2D,
};

typedef union nn_LayerUnion nn_LayerUnion;
union nn_LayerUnion {
  nn_LayerDense dense_layer;
  nn_LayerNorm norm_layer;
  nn_LayerConv1D conv1D_layer;
  nn_LayerConv2D conv2D_layer;
};

typedef struct nn_Layer nn_Layer;
struct nn_Layer {
  layerType type;
  nn_LayerUnion layer_u;
};

union _nn_LayerCreationArguments {
  struct _nn_LayerCreationArguments_Dense   dense_args;
  struct _nn_LayerCreationArguments_Norm     norm_args;
  struct _nn_LayerCreationArguments_Conv1D conv1D_args;
  struct _nn_LayerCreationArguments_Conv2D conv2D_args;
};

// input layer is always a dense layer
nn_Layer* _nn_createInputLayer(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape);
// for hidden layers
nn_Layer* _nn_createLayer(layerType type, union _nn_LayerCreationArguments* args);
// output layer is always a dense layer
nn_Layer* _nn_createOutputLayer(nn_ShapeDescription layer_shape, activation activation);
// free the allocated memory
void _nn_freeLayer(nn_Layer* layer);

#endif
