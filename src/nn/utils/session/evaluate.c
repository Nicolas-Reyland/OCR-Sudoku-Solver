// evaluate .c

#include "evaluate.h"

void _nn_fastForward(nn_ModelLayers* model_layers, double* input)
{
  // feed input layer with input
  for (size_t i = 0; i < model_layers->input_layer.nb_nodes; i++) {
    model_layers->input_layer.nodes[i]->value = input[i];
  }
  // process middle-layers (if there are any)
  nn_Layer* last_layer;
  if (model_layers->num_hidden_layers) {
    _nn_fastForwardLayer(&model_layers->input_layer, &model_layers->hidden_layers[0]);
    for (unsigned int i = 1; i < model_layers->num_hidden_layers; i++) {
      _nn_fastForwardLayer(&model_layers->hidden_layers[i-1], &model_layers->hidden_layers[i]);
    }
    last_layer = &model_layers->hidden_layers[model_layers->num_hidden_layers - 1];
  } else {
    last_layer = &model_layers->input_layer;
  }
  // fast forward output_layer
  _nn_fastForwardLayer(last_layer, &model_layers->output_layer);
}

void _nn_fastForwardLayer(nn_Layer* from_layer, nn_Layer* to_layer)
{
  // have to verify first that the nodes do all have values
  for (size_t i = 0; i < to_layer->nb_nodes; i++) {
    double sum = 0;
    for (size_t j = 0; j < from_layer->nb_nodes; j++) {
      sum += to_layer->nodes[i]->weights[j] * from_layer->nodes[j]->value;
    }
    to_layer->nodes[i]->raw_value = sum + from_layer->nodes[i]->bias;
    to_layer->nodes[i]->value = 0.0/0.0; // nan
  }
  _nn_activateLayer(to_layer);
}

void _nn_backPropagation(nn_Layer* from_layer, nn_Layer* to_layer);

//
