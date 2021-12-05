// layer.h

#ifndef NN_LAYER_H
#define NN_LAYER_H

#include "nn/functions_descriptors/functions_descriptors_enums.h"
#include "nn/utils/structs/shape_description.h"
#include "node/node.h"
#include <stdbool.h>

typedef struct nn_Layer nn_Layer;
struct nn_Layer {
    nn_ShapeDescription shape;
    activation activation;
    size_t num_nodes;
    nn_Node **nodes;
};

nn_Layer *_nn_createInputLayer(nn_ShapeDescription layer_shape,
                               nn_ShapeDescription next_layer_shape);
nn_Layer *_nn_createLayer(nn_ShapeDescription layer_shape,
                          nn_ShapeDescription next_layer_shape,
                          activation activation);
nn_Layer *_nn_createOutputLayer(nn_ShapeDescription layer_shape,
                                activation activation);
void _nn_freeLayer(nn_Layer *layer);

#endif
