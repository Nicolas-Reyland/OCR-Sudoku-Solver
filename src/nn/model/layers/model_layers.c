// model_layers.c

#include "model_layers.h"

void _nn_printModelLayers(nn_ModelLayers* model_layers)
{
	nn_Layer* layer;
	for (size_t index = 0; index < model_layers->num_hidden_layers + 2; index++) {
		layer = index == 0 ? &model_layers->input_layer : (index == model_layers->num_hidden_layers + 1 ? &model_layers->output_layer : &model_layers->hidden_layers[index - 1]);
		printf("index: %ld\n", index);
		nn_Node* node;
		for (size_t i = 0; i < layer->nb_nodes; i++) {
			node = layer->nodes[i];
			for (size_t j = 0; j < node->num_weights; j++) {
				printf("layers[%ld]->nodes[%ld]->weights[%ld] = %lf\n", index, i, j, node->weights[j]);
			}
			printf("bias: %lf\n", node->bias);
		}
	}
}

void _nn_printModelLayersValues(nn_ModelLayers* model_layers)
{
	nn_Layer* layer;
	for (size_t index = 0; index < model_layers->num_hidden_layers + 2; index++) {
		layer = index == 0 ? &model_layers->input_layer : (index == model_layers->num_hidden_layers + 1 ? &model_layers->output_layer : &model_layers->hidden_layers[index - 1]);
		nn_Node* node;
		for (size_t i = 0; i < layer->nb_nodes; i++) {
			node = layer->nodes[i];
			printf("layers[%ld]->nodes[%ld]->value = %lf\n", index, i, node->value);
			printf("layers[%ld]->nodes[%ld]->raw_value = %lf\n", index, i, node->raw_value);
		}
	}
}

nn_ModelLayers* _nn_createModelLayers(size_t num_hidden_layers, nn_ShapeDescription model_architecture[], activation activations[])
{
  // input & output layers
  nn_Layer* input_layer = _nn_createInputLayer(model_architecture[0], model_architecture[1]);
  nn_Layer* output_layer = _nn_createOutputLayer(model_architecture[num_hidden_layers + 1], activations[num_hidden_layers]);
  // hidden layers
  nn_Layer* hidden_layers = NULL;
  if (num_hidden_layers) {
    hidden_layers = mem_calloc(num_hidden_layers, sizeof(nn_Layer));
    for (size_t i = 0; i < num_hidden_layers; i++) {
      verbose("Allocating hidden layer ...");
      nn_Layer* hidden_layer = _nn_createLayer(model_architecture[i + 1], model_architecture[i + 2], activations[i]);
      hidden_layers[0] = *hidden_layer;
    }
  }
  verbose("layers are allocated.");
  // malloc struct
  nn_ModelLayers* model_layers = mem_malloc(sizeof(nn_ModelLayers));
  model_layers->input_layer = *input_layer;
  model_layers->num_hidden_layers = num_hidden_layers;
  model_layers->hidden_layers = hidden_layers;
  model_layers->output_layer = *output_layer;

  //utils
  model_layers->printModelLayers        = &_nn_printModelLayers;
  model_layers->printModelLayersValues  = &_nn_printModelLayersValues;
  return model_layers;
}

void _nn_freeModelLayers(nn_ModelLayers* model_layers)
{
  _nn_freeLayer(&model_layers->input_layer);
  verbose("freed input layer");
  _nn_freeLayer(&model_layers->output_layer);
  verbose("freed output layer");
  for (size_t i = 0; i < model_layers->num_hidden_layers; i++) {
    verbose("freeing hidden layer num %d", i);
    _nn_freeLayer(&model_layers->hidden_layers[i]);
  }
  mem_free(model_layers->hidden_layers);
  mem_free(model_layers);
}

//
