// model_layer.c

#include "model_layer.h"
#include <stdlib.h>
#include <stdio.h>

nn_ModelLayers* _nn_createModelLayer(unsigned int num_hidden_layers, ShapeDescription model_architecture[], activationFunction activations[])
{
  // input & output layers
  nn_Layer* input_layer = _nn_createLayer(model_architecture[0], model_architecture[1], activations[0]);
  nn_Layer* output_layer = _nn_createLayer(model_architecture[num_hidden_layers + 1], model_architecture[num_hidden_layers + 2], activations[num_hidden_layers + 1]);
  printf("created input and output layer\n");
  // hidden layers
  nn_Layer* hidden_layers = NULL;
  if (num_hidden_layers) {
    hidden_layers = calloc(num_hidden_layers, sizeof(nn_Layer));
    for (unsigned int i = 0; i < num_hidden_layers; i++) {
      nn_Layer* hidden_layer = _nn_createLayer(model_architecture[i + 1], model_architecture[i + 2], activations[i + 1]);
      printf("hidden layer %u created\n", i);
      hidden_layers[0] = *hidden_layer;
    }
  }
  printf("created hidden layers\n");
  // malloc struct
  nn_ModelLayers* model_layers = malloc(sizeof(nn_ModelLayers));
  model_layers->input_layer = *input_layer;
  model_layers->num_hidden_layers = num_hidden_layers;
  model_layers->hidden_layers = hidden_layers;
  model_layers->output_layer = *output_layer;
  return model_layers;
}

//
