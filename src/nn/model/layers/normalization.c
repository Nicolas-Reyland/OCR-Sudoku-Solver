// normalization.c

#include "normalization.h"

nn_LayerNorm* _nn_createLayerNorm(struct _nn_LayerCreationArguments_Norm* args)
{
  // malloc layer
  nn_LayerNorm* norm_layer = mem_malloc(sizeof(nn_LayerNorm));
  // assign attribtues
  norm_layer->layer_shape = args->layer_shape;
  norm_layer->epsilon = args->epsilon;
  return norm_layer;
}

void _nn_freeLayerNorm(nn_LayerNorm* norm_layer)
{
  mem_free(norm_layer);
}
