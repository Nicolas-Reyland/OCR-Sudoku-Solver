// layer.c

#include "layer.h"

nn_Layer *_nn_createLayerWrapped(nn_ShapeDescription layer_shape,
                                 nn_ShapeDescription next_layer_shape,
                                 activation activation, bool alloc_weights,
                                 bool init_bias) {
    size_t num_nodes = layer_shape.range;
    size_t num_nodes_next_layer = alloc_weights ? next_layer_shape.range : 0;
    // create nodes
    nn_Node **nodes = mem_calloc(num_nodes, sizeof(nn_Node *));
    for (size_t z = 0; z < layer_shape.z; z++) {
        for (size_t y = 0; y < layer_shape.y; y++) {
            for (size_t x = 0; x < layer_shape.x; x++) {
                nn_Node *node = _nn_createNode(
                    alloc_weights ? num_nodes_next_layer : 0, init_bias);
                nodes[z * layer_shape.y * layer_shape.x + y * layer_shape.x +
                      x] = node;
            }
        }
    }
    // malloc struct
    nn_Layer *layer = mem_malloc(sizeof(nn_Layer));
    layer->shape = layer_shape;
    layer->activation = activation;
    layer->num_nodes = num_nodes;
    layer->nodes = nodes;
    return layer;
}

nn_Layer *_nn_createInputLayer(nn_ShapeDescription layer_shape,
                               nn_ShapeDescription next_layer_shape) {
    return _nn_createLayerWrapped(layer_shape, next_layer_shape, NO_ACTIVATION,
                                  true, false);
}

nn_Layer *_nn_createLayer(nn_ShapeDescription layer_shape,
                          nn_ShapeDescription next_layer_shape,
                          activation activation) {
    return _nn_createLayerWrapped(layer_shape, next_layer_shape, activation,
                                  true, true);
}

nn_Layer *_nn_createOutputLayer(nn_ShapeDescription layer_shape,
                                activation activation) {
    return _nn_createLayerWrapped(layer_shape, layer_shape, activation, false,
                                  true);
}

void _nn_freeLayer(nn_Layer *layer) {
    for (size_t i = 0; i < layer->shape.range; i++) {
        nn_Node *node = layer->nodes[i];
        _nn_freeNode(node);
    }
    mem_free(layer->nodes);
}

//
