// model_layers.c

#include "model_layers.h"

//#include <stdio.h>

nn_ModelLayers* _nn_createModelLayers(unsigned int num_hidden_layers, nn_ShapeDescription model_architecture[], activation activations[])
{
  // input & output layers
  nn_Layer* input_layer = _nn_createLayer(model_architecture[0], model_architecture[1], activations[0]);
  nn_Layer* output_layer = _nn_createLayer(model_architecture[num_hidden_layers], model_architecture[num_hidden_layers + 1], activations[num_hidden_layers + 1]);
  // hidden layers
  nn_Layer* hidden_layers = NULL;
  if (num_hidden_layers) {
    hidden_layers = mem_calloc(num_hidden_layers, sizeof(nn_Layer));
    for (unsigned int i = 0; i < num_hidden_layers; i++) {
      //printf("Allocating hidden layer ...\n");
      nn_Layer* hidden_layer = _nn_createLayer(model_architecture[i + 1], model_architecture[i + 2], activations[i + 1]);
      hidden_layers[0] = *hidden_layer;
    }
  }
  //printf("layers are allocated.\n");
  // malloc struct
  nn_ModelLayers* model_layers = mem_malloc(sizeof(nn_ModelLayers));
  model_layers->input_layer = *input_layer;
  model_layers->num_hidden_layers = num_hidden_layers;
  model_layers->hidden_layers = hidden_layers;
  model_layers->output_layer = *output_layer;

  return model_layers;
}

void _nn_freeModelLayers(nn_ModelLayers* model_layers)
{
  _nn_freeLayer(&model_layers->input_layer);
  _nn_freeLayer(&model_layers->output_layer);
  for (unsigned int i = 0; i < model_layers->num_hidden_layers; i++) {
    _nn_freeLayer(&model_layers->hidden_layers[i]);
  }
  mem_free(model_layers->hidden_layers);
  mem_free(model_layers);
}

//
