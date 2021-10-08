// layer.c

#include "layer.h"

nn_Layer* _nn_createLayer(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activationFunction activation_function)
{
  unsigned int num_next_layer_weights = next_layer_shape.x * next_layer_shape.y * next_layer_shape.z;
  // create nodes
  nn_Node** nodes = calloc(layer_shape.x * layer_shape.y * layer_shape.z, sizeof(nn_Node*));
  for (unsigned int z = 0; z < layer_shape.z; z++) {
    for (unsigned int y = 0; y < layer_shape.y; y++) {
      for (unsigned int x = 0; x < layer_shape.x; x++) {
        nn_Node* node = _nn_createNode(num_next_layer_weights);
        nodes[
          + x
          + y * layer_shape.y
          + z * layer_shape.y * layer_shape.x
        ] = node;
      }
    }
  }
  // malloc struct
  nn_Layer* layer = malloc(sizeof(nn_Layer));
  layer->shape = layer_shape;
  layer->activation_function = activation_function;
  layer->nodes = nodes;
  return layer;
}

//
