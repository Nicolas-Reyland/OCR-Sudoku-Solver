// conv1D.h

#ifndef NN_CONV1D_H
#define NN_CONV1D_H

typedef struct nn_LayerConv1D nn_LayerConv1D;
struct nn_LayerConv1D {
  nn_ShapeDescription layer_shape;
  size_t kernel_size;
  size_t strides;
};

struct _nn_LayerCreationArguments_Conv1D {
  nn_ShapeDescription layer_shape;
  size_t kernel_size;
  size_t strides;
};

nn_LayerConv1D* _nn_createLayerConv1D(struct _nn_LayerCreationArguments_Conv1D* args);
void _nn_freeLayerConv1D(nn_LayerConv1D* conv1D_layer);

#endif
