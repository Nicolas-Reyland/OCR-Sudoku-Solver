// layer.c

#include "layer.h"

nn_Layer* _nn_createLayerWrapped(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activation activation, bool alloc_weights)
{
  size_t num_next_layer_weights = alloc_weights ? next_layer_shape.x * next_layer_shape.y * next_layer_shape.z : 0;
  // create nodes
  nn_Node** nodes = mem_calloc(layer_shape.x * layer_shape.y * layer_shape.z, sizeof(nn_Node*));
  for (size_t z = 0; z < layer_shape.z; z++) {
    for (size_t y = 0; y < layer_shape.y; y++) {
      for (size_t x = 0; x < layer_shape.x; x++) {
        nn_Node* node = _nn_createNode(num_next_layer_weights);
        nodes[
          z * layer_shape.y * layer_shape.x
          + y * layer_shape.x
          + x
        ] = node;
      }
    }
  }
  verbose("Done allocating nodes");
  // malloc struct
  nn_Layer* layer = mem_malloc(sizeof(nn_Layer));
  layer->shape = layer_shape;
  layer->activation = activation;
  layer->nodes = nodes;
  layer->nb_nodes = layer_shape.x * layer_shape.y * layer_shape.z;
  return layer;
}

nn_Layer* _nn_createLayer(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activation activation)
{
  return _nn_createLayerWrapped(layer_shape, next_layer_shape, activation, true);
}

nn_Layer* _nn_createOutputLayer(nn_ShapeDescription layer_shape, activation activation)
{
  return _nn_createLayerWrapped(layer_shape, layer_shape, activation, false);
}

void _nn_freeLayer(nn_Layer* layer)
{
  verbose("freeing layer ...");
  for (size_t i = 0; i < layer->shape.x * layer->shape.y * layer->shape.z; i++) {
    nn_Node* node = layer->nodes[i];
    _nn_freeNode(node);
  }
  verbose("layer has been freed");
  mem_free(layer->nodes);
}

//
