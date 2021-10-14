// layer.c

#include "layer.h"

nn_Layer* _nn_createLayer(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activation activation)
{
  unsigned int num_next_layer_weights = next_layer_shape.x * next_layer_shape.y * next_layer_shape.z;
  // create nodes
  nn_Node** nodes = calloc(layer_shape.x * layer_shape.y * layer_shape.z, sizeof(nn_Node*));
  for (unsigned int z = 0; z < layer_shape.z; z++) {
    for (unsigned int y = 0; y < layer_shape.y; y++) {
      for (unsigned int x = 0; x < layer_shape.x; x++) {
        //printf("Allocating node %d %d %d\n", x, y, z);
        nn_Node* node = _nn_createNode(num_next_layer_weights);
        nodes[
          + x
          + y * layer_shape.y
          + z * layer_shape.y * layer_shape.x
        ] = node;
      }
    }
  }
  //printf("Done allocating nodes\n");
  // malloc struct
  nn_Layer* layer = malloc(sizeof(nn_Layer));
  layer->shape = layer_shape;
  layer->activation = activation;
  layer->nodes = nodes;
  return layer;
}

void _nn_freeLayer(nn_Layer* layer)
{
  for (unsigned int i = 0; i < layer->shape.x * layer->shape.y * layer->shape.z; i++) {
    nn_Node* node = layer->nodes[i];
    _nn_freeNode(node);
  }
  mem_free(layer->nodes);
}

//
