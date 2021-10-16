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
    for (size_t i = 1; i < model_layers->num_hidden_layers; i++) {
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
    // init value at nan
    to_layer->nodes[i]->value = 0.0/0.0;
    to_layer->nodes[i]->raw_value = to_layer->nodes[i]->bias;
    // calculate the rest of the raw_value (using the weights)
    for (size_t j = 0; j < from_layer->nb_nodes; j++) {
      to_layer->nodes[i]->raw_value += to_layer->nodes[i]->weights[j] * from_layer->nodes[j]->value;
    }
  }
  _nn_activateLayer(to_layer);
}

void _nn_backPropagation(nn_ModelLayers* model_layers, double* desired_output)
{
  // https://medium.com/analytics-vidhya/building-neural-network-framework-in-c-using-backpropagation-8ad589a0752d

  // Calculate output layer derivative weights, values etc.
  nn_Layer* output_layer = &model_layers->output_layer;
  for (size_t j = 0; output_layer->nb_nodes; j++) {
    // calculate derivative raw_value of output_layer
    output_layer->nodes[j]->delta_value = (output_layer->nodes[j]->value - desired_output[j]) * _nn_derivativeActivation(output_layer->nodes[j]->raw_value, output_layer->activation);
    // derivative weights of output_layer
    nn_Layer* layer_before_output;
    if (model_layers->num_hidden_layers) {
      layer_before_output = &model_layers->hidden_layers[model_layers->num_hidden_layers - 1];
    } else {
      layer_before_output = &model_layers->input_layer;
    }
    for (size_t k = 0; k < layer_before_output->nb_nodes; k++) {
      layer_before_output->nodes[k]->d_weights[j] = output_layer->nodes[j]->delta_value * layer_before_output->nodes[k]->value;
      layer_before_output->nodes[k]->d_value = layer_before_output->nodes[k]->weights[j] * output_layer->nodes[j]->delta_value;
    }
    // derivative bias
    output_layer->nodes[j]->d_bias = output_layer->nodes[j]->delta_value;
  }

  // Propagate backwards towards the hidden layers
  nn_Layer* current_layer, * next_layer;
  /* ATTENTION: next_layer is the next layer looking at the model BACKWARDS.
   * So if current_layer is the layer at index 'i', next_layer is at the index 'i - 1' (towards the input layer)
   */
  for (size_t i = model_layers->num_hidden_layers - 1; i >= 0; i--) { // for (...yers - 1 ; i-- > 0; )
    current_layer = &model_layers->hidden_layers[i];
    next_layer = i == 0 ? &model_layers->input_layer : &model_layers->hidden_layers[i - 1];
    // calculate derivatives for current layer
    for (size_t j = 0; j < current_layer->nb_nodes; j++) {
      current_layer->nodes[j]->delta_value = current_layer->nodes[j]->d_value * _nn_derivativeActivation(current_layer->nodes[j]->raw_value, current_layer->activation);
      // calculate derivatives for next layer
      for (size_t k = 0; k < next_layer->nb_nodes; k++) {
        next_layer->nodes[k]->d_weights[j] = current_layer->nodes[j]->delta_value * next_layer->nodes[k]->value;
        if (i > 0) {
          next_layer->nodes[k]->d_value = next_layer->nodes[k]->weights[j] * current_layer->nodes[j]->delta_value;
        }
      }
      current_layer->nodes[j]->d_bias = current_layer->nodes[j]->delta_value;
    }
  }
}

void _nn_updateWeights(nn_ModelLayers* model_layers)
{
  // for (size_t i = 0; i < model_layers->)
}
















//
