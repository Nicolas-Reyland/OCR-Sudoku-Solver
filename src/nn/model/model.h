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

/*
void _nn_Model_saveArchitectureFn(char*);
void _nn_Model_saveWeightsAndBias(char*);
nn_Model _nn_Model_loadFn(char*);
*/

#endif
