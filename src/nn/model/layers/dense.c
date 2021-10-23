// dense.c

#include "dense.h"

nn_LayerDense* _nn_createLayerDense(struct _nn_LayerCreationArguments_Dense* args)
{
  // extract arguments
  nn_ShapeDescription layer_shape = args->layer_shape;
  nn_ShapeDescription next_layer_shape = args->next_layer_shape;
  activation activation = args->activation;
  bool alloc_weights = args->alloc_weights;
  bool init_bias = args->init_bias;
  //
  size_t num_nodes = layer_shape.range;
  size_t num_nodes_next_layer = alloc_weights ? next_layer_shape.range : 0;
  // create nodes
  nn_Node** nodes = mem_calloc(num_nodes, sizeof(nn_Node*));
  for (size_t z = 0; z < layer_shape.z; z++) {
    for (size_t y = 0; y < layer_shape.y; y++) {
      for (size_t x = 0; x < layer_shape.x; x++) {
        nn_Node* node = _nn_createNode(args->alloc_weights ? num_nodes_next_layer : 0, init_bias);
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
  nn_LayerDense* dense_layer = mem_malloc(sizeof(nn_LayerDense));
  dense_layer->shape = layer_shape;
  dense_layer->activation = activation;
  dense_layer->num_nodes = num_nodes;
  dense_layer->nodes = nodes;
  return dense_layer;
}

void _nn_freeLayerDense(nn_LayerDense* layer)
{
  verbose("freeing dense layer ...");
  for (size_t i = 0; i < layer->shape.range; i++) {
    nn_Node* node = layer->nodes[i];
    _nn_freeNode(node);
  }
  verbose("dense layer has been freed");
  mem_free(layer->nodes);
}


