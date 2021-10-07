// layer.h

#ifndef NN_LAYER_H
#define NN_LAYER_H

#include <stdbool.h>
#include "node.h"
#include "../../utils/structs/shape_description.h"
#include "../../functions_descriptors/functions_descriptors_enums.h"

typedef struct nn_Layer {
  ShapeDescription shape;
  activationFunction activation_function;
  nn_Node* nodes;
} nn_Layer;

nn_Layer* _nn_createLayer(ShapeDescription layer_shape, ShapeDescription next_layer_shape, activationFunction activation_function);

#endif
