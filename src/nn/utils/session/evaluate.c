// evaluate .c

#include "evaluate.h"

void _nn_fast_forward(nn_Model* model, double* input)
{
  //
}

void _nn_fast_forward_layer(nn_Layer* from_layer, nn_Layer* to_layer)
{
  // have to verify first that the nodes do all have values
  for (size_t i = 0; i < to_layer->nb_nodes; i++) {
    double sum = 0;
    for (size_t j = 0; j < from_layer->nb_nodes; j++) {
      sum += to_layer->nodes[i]->weights[j] * from_layer->nodes[j]->value;
    }
    to_layer->nodes[i]->raw_value = sum + from_layer->bias[i];
    to_layer->nodes[i]->value = 0.0/0.0; // nan
  }
  _nn_activateLayer(to_layer);
}

void _nn_backward_propagation(nn_Layer* from_layer, nn_Layer* to_layer);

//
