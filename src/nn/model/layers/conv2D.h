// conv2D.h

#ifndef NN_CONV2D_H
#define NN_CONV2D_H

typedef struct nn_LayerConv2D nn_LayerConv2D;
struct nn_LayerConv2D {
  nn_ShapeDescription layer_shape;
  nn_ShapeDescription kernel_size;
  nn_ShapeDescription strides;
};

struct _nn_LayerCreationArguments_Conv2D {
  nn_ShapeDescription layer_shape;
  nn_ShapeDescription kernel_size;
  nn_ShapeDescription strides;
};

nn_LayerConv2D* _nn_createLayerConv2D(struct _nn_LayerCreationArguments_Conv2D* args);
void _nn_freeLayerConv2D(nn_LayerConv2D* conv2D_layer);

#endif
