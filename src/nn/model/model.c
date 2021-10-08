// model.c
#include "model.h"
#include <stdio.h>

void _nn_Model_saveArchitectureFn(nn_Model* model, char* path)
{
  //
}

void _nn_Model_saveWeightsAndBias(nn_Model* model, char* path)
{
  //
}

void _nn_Model_saveModel(nn_Model* model, char* dirpath)
{
  //
}

nn_Model* createModel(unsigned int num_layers, ShapeDescription model_architecture[], activationFunction activations[], lossFunction loss, optimizer optimizer)
{
  nn_ModelLayers* layers = _nn_createModelLayer(num_layers - 2, model_architecture, activations);
  // malloc struct
  nn_Model* model = malloc(sizeof(nn_Model));
  model->layers = layers;
  model->loss = loss;
  model->optimizer = optimizer;
  // add functions to struct
  model->saveArchitecture = &_nn_Model_saveArchitectureFn;
  model->saveWeightsAndBias = &_nn_Model_saveWeightsAndBias;
  model->saveModel = &_nn_Model_saveModel;
  return model;
}

// nn_Model* loadModel(char* path);
