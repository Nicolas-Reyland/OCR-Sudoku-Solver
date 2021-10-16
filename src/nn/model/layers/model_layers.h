// model_layers.h

#ifndef NN_MODEL_LAYERS_H
#define NN_MODEL_LAYERS_H

#include "layer.h"
#include <stdlib.h>

typedef struct nn_ModelLayers {
  nn_Layer input_layer;
  size_t num_hidden_layers;
  nn_Layer* hidden_layers;
  nn_Layer output_layer;
} nn_ModelLayers;

nn_ModelLayers* _nn_createModelLayers(unsigned int num_hidden_layers, nn_ShapeDescription model_architecture[], activation activations[]);
void _nn_freeModelLayers(nn_ModelLayers* model_layers);

#endif
