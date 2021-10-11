// model.h

#ifndef NN_MODEL_H
#define NN_MODEL_H

#include "layers/model_layer.h"
#include "../functions_descriptors/functions_descriptors_enums.h"

typedef struct nn_Model {
  nn_ModelLayers* layers;
  losses loss;
  optimizer optimizer;
  void (*saveArchitecture)(struct nn_Model* model, char*);
  void (*saveWeightsAndBias)(struct nn_Model* model, char*);
  void (*saveModel)(struct nn_Model* model, char*);
} nn_Model;

nn_Model* createModel(unsigned int num_layers, nn_ShapeDescription model_architecture[], activation activations[], losses loss, optimizer optimizer);
nn_Model* loadModel(char* path);

#endif
