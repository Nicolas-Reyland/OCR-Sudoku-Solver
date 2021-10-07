// model.c
#include "model.h"


void _nn_Model_saveArchitectureFn(char* path)
{
  //
}

void _nn_Model_saveWeightsAndBias(char* path)
{
  //
}

void _nn_Model_saveModel(char* dirpath)
{
  //
}

nn_Model createModel(nn_ModelLayers layers, lossFunction loss, optimizer optimizer)
{
  nn_Model model = {
    .layers = layers,
    .loss = loss,
    .optimizer = optimizer
  };
  model.saveArchitecture = &_nn_Model_saveArchitectureFn;
  model.saveWeightsAndBias = &_nn_Model_saveWeightsAndBias;
  model.saveModel = &_nn_Model_saveModel;
  return model;
}

// nn_Model loadModel(char* path);
