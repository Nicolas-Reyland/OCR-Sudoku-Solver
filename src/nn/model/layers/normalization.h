// normalization.h

#ifndef NN_NORMALIZATION_H
#define NN_NORMALIZATION_H

#include "utils/mem/mem-management.h"
#include "nn/utils/structs/shape_description.h"

typedef struct nn_LayerNorm nn_LayerNorm;
struct nn_LayerNorm {
  nn_ShapeDescription shape;
  double epsilon;
};

struct _nn_LayerCreationArguments_Norm {
  nn_ShapeDescription layer_shape;
  double epsilon;
};

nn_LayerNorm* _nn_createLayerNorm(struct _nn_LayerCreationArguments_Norm* args);
void _nn_freeLayerNorm(nn_LayerNorm* norm_layer);

#endif
