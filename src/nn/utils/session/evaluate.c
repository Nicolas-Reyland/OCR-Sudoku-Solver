// evaluate .c

#include "evaluate.h"

void _nn_feedForward(nn_ModelLayers* model_layers, double* input)
{
  // feed input layer with input
  verbose("feeding the input layer");
  for (size_t i = 0; i < model_layers->input_layer.nb_nodes; i++) {
    model_layers->input_layer.nodes[i]->value = input[i];
  }
  // process middle-layers (if there are any)
  nn_Layer* last_layer;
  if (model_layers->num_hidden_layers) {
    verbose("feeding the hidden layers");
    _nn_feedForwardLayer(&model_layers->input_layer, &model_layers->hidden_layers[0]);
    for (size_t i = 1; i < model_layers->num_hidden_layers; i++) {
      _nn_feedForwardLayer(&model_layers->hidden_layers[i-1], &model_layers->hidden_layers[i]);
    }
    last_layer = &model_layers->hidden_layers[model_layers->num_hidden_layers - 1];
  } else {
    last_layer = &model_layers->input_layer;
  }
  // fast forward output_layer
  verbose("feeding the last 'feedable' layer %p", last_layer);
  _nn_feedForwardLayer(last_layer, &model_layers->output_layer);
}

void _nn_feedForwardLayer(nn_Layer* from_layer, nn_Layer* to_layer)
{
  // have to verify first that the nodes do all have values
  for (size_t i = 0; i < to_layer->nb_nodes; i++) {
    // init raw value at bias
    to_layer->nodes[i]->raw_value = to_layer->nodes[i]->bias;
    // calculate the rest of the raw_value (using the weights)
    for (size_t j = 0; j < from_layer->nb_nodes; j++) {
      to_layer->nodes[i]->raw_value += from_layer->nodes[j]->weights[i] * from_layer->nodes[j]->value;
    }
  }
  _nn_activateLayer(to_layer);
}

void _nn_backPropagation(nn_Model* model, double* desired_output)
{
  // https://medium.com/analytics-vidhya/building-neural-network-framework-in-c-using-backpropagation-8ad589a0752d
  
  // get model layers struct
  nn_ModelLayers* model_layers = model->layers;

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
  for (size_t i = model_layers->num_hidden_layers - 1; /* manually breaking btw */; i--) { // for (...yers - 1 ; i-- > 0; )
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
    if (i == 0) {
      break; // cannot check if i <= 0 bc i has type size_t
    }
  }
}

void _nn_updateWeights(nn_ModelLayers* model_layers, float learning_rate)
{
  //updating weights and bias of input layer
  for(size_t i = 0; i < model_layers->input_layer.nb_nodes;i++)
  {
    nn_Node* node = model_layers->input_layer.nodes[i];
    for(size_t j = 0; j < model_layers->hidden_layers[0].nb_nodes ;j++)
    {
      node->weights[j] = node->weights[j] - (learning_rate*node->d_weights[j]);
    }
    node->bias = node->bias - (learning_rate * node->d_bias);
  }

  //updating weights and bias of hidden layers
  for(size_t i = 0; i < model_layers->num_hidden_layers-1; i++)
  {
    for(size_t j = 0; j < model_layers->hidden_layers[i].nb_nodes; j++)
    {
      nn_Layer* layer = &(model_layers->hidden_layers[i]);
      for(size_t k = 0; k < model_layers->hidden_layers[i+1].nb_nodes;k++)
      {
        //updating weight
        layer->nodes[j]->weights[k] = layer->nodes[j]->weights[k] - 
          (learning_rate * layer->nodes[j]->d_weights[k]);
      }
      // update bias
      layer->nodes[j]->bias = layer->nodes[j]->bias - 
        (learning_rate * layer->nodes[j]->d_bias);
    }
  }
}
















//
