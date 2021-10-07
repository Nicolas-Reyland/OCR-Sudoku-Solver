// model.h

#ifndef NN_MODEL_H
#define NN_MODEL_H

#include "layers/model_layer.h"
#include "../functions_descriptors/functions_descriptors_enums.h"

typedef struct nn_Model {
  nn_ModelLayers layers;
  lossFunction loss;
  optimizer optimizer;
  void (*saveArchitecture)(char*);
  void (*saveWeightsAndBias)(char*);
  void (*saveModel)(char*);
} nn_Model;

nn_Model createModel(nn_ModelLayers layers, lossFunction loss, optimizer optimizer);
nn_Model loadModel(char* path);

#endif
