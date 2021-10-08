// model_layer.h

#ifndef NN_MODEL_LAYER_H
#define NN_MODEL_LAYER_H

#include "layer.h"

typedef struct nn_ModelLayers {
  nn_Layer input_layer;
  unsigned int num_hidden_layers;
  nn_Layer* hidden_layers;
  nn_Layer output_layer;
} nn_ModelLayers;

nn_ModelLayers* _nn_createModelLayer(unsigned int num_hidden_layers, nn_ShapeDescription model_architecture[], activationFunction activations[]);

#endif
