// layer.c

#include "layer.h"

nn_Layer* _nn_createLayer(layerType type, union _nn_LayerCreationArguments* args) // layerType type, nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activation activation, bool alloc_weights, bool init_bias)
{
  nn_Layer* layer;
  switch (type) {
    case DENSE:
      layer->type = DENSE;
      nn_LayerDense* dense_layer = _nn_createLayerDense(&args->dense_args);
      layer->layer_u.dense_layer = dense_layer;
      return layer;
    case NORMALIZATION:
      layer->type = NORMALIZATION;
      nn_LayerNorm* norm_layer = _nn_createLayerNorm(&args->norm_args);
      layer->layer_u.norm_layer = norm_layer;
      return layer;
    case CONV1D:
      layer->type = CONV1D;
      nn_LayerConv1D* conv1D_layer = _nn_createLayerConv1D(&args->conv1D_args);
      layer->layer_u.conv1D_layer = conv1D_layer;
      return layer;
    case CONV2D:
      layer->type = CONV2D;
      nn_LayerConv2D* conv2D_layer = _nn_createLayerConv2D(&args->conv2D_args);
      layer->layer_u.conv2D_layer = conv2D_layer;
      return layer;
    default:
      fprintf(stderr, "Unknown layer type: %d\n", type);
      exit(EXIT_FAILURE);
      return NULL;
  }
}

nn_Layer* _nn_createInputLayer(nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape)
{
  union _nn_LayerCreationArguments args;
  struct _nn_LayerCreationArguments_Dense dense_args = {
    .layer_shape = layer_shape,
    .next_layer_shape = next_layer_shape,
    .activation = NO_ACTIVATION,
    .alloc_weights = true,
    .init_bias = false,
  };
  args.dense_args = dense_args;
  return _nn_createLayer(DENSE, &args);
}

/*nn_Layer* _nn_createLayer(layerType type, nn_ShapeDescription layer_shape, nn_ShapeDescription next_layer_shape, activation activation)
{
  union _nn_LayerCreationArguments args;
  args._nn_LayerCreationArguments_Dense = {
    .layer_shape = layer_shape,
    .next_layer_shape = next_layer_shape,
    .activation = NO_ACTIVATION,
    .alloc_weights = true,
    .init_bias = true,
  };
  return _nn_createLayer_Wrapped(type, );
}*/

nn_Layer* _nn_createOutputLayer(nn_ShapeDescription layer_shape, activation activation)
{
  union _nn_LayerCreationArguments args;
  struct _nn_LayerCreationArguments_Dense dense_args = {
    .layer_shape = layer_shape,
    .next_layer_shape = layer_shape,
    .activation = NO_ACTIVATION,
    .alloc_weights = false,
    .init_bias = true,
  };
  args.dense_args = dense_args;
  return _nn_createLayer(DENSE, &args);
}

void _nn_freeLayer(nn_Layer* layer)
{
  // free inner layer
  switch (layer->type) {
    case DENSE:
      _nn_freeLayerDense(&layer->layer_u.dense_layer);
      break;
    case NORMALIZATION:
      _nn_freeLayerNorm(&layer->layer_u.norm_layer);
    case CONV1D:
      _nn_freeLayerConv1D(&layer->layer_u.conv1D_layer);
      break;
    case CONV2D:
      _nn_freeLayerConv2D(&layer->layer_u.conv2D_layer);
      break;
    default:
      fprintf(stderr, "Unknown layer type: %d\n", layer->type);
      exit(EXIT_FAILURE);
      return;
  }
  // free layer wrapper struct
  mem_free(layer);
}

//
