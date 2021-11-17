// model.h

#ifndef NN_MODEL_H
#define NN_MODEL_H

#include <string.h>
#include "layers/layer.h"
#include "nn/functions_descriptors/functions_descriptors_enums.h"
#include "utils/mem/mem-management.h"

typedef struct nn_Model {
  size_t num_layers;
  nn_Layer** layers;
  losses loss;
  optimizer optimizer;
  void (*printModelLayers)(struct nn_Model* model);
  void (*printModelLayersValues)(struct nn_Model* model);
  void (*saveArchitecture)(struct nn_Model* model, char*);
  void (*saveWeightsAndBias)(struct nn_Model* model, char*);
  void (*saveModel)(struct nn_Model* model, char*);
} nn_Model;

nn_Model* createModel(size_t num_layers, nn_ShapeDescription model_architecture[], activation activations[], losses loss, optimizer optimizer);
void freeModel(nn_Model* model);
nn_Model* nn_loadModel(char* path);

#endif
