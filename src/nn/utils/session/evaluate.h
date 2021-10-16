// evaluate.h

#ifndef NN_EVALUATE_H
#define NN_EVALUATE_H

#include "nn/model/model.h"
#include "nn/utils/session/functions/activations.h"

void _nn_feedForward(nn_ModelLayers* model, double* input);
void _nn_feedForwardLayer(nn_Layer* from_layer, nn_Layer* to_layer);
void _nn_backPropagation(nn_Model* model, double* desired_output);
void _nn_updateWeights(nn_ModelLayers* model_layers);

#endif
