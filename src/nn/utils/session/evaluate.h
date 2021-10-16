// evaluate.h

#ifndef NN_EVALUATE_H
#define NN_EVALUATE_H

#include "nn/model/model.h"
#include "nn/utils/session/functions/activations.h"

void _nn_fastForward(nn_ModelLayers* model, double* input);
void _nn_fastForwardLayer(nn_Layer* from_layer, nn_Layer* to_layer);
void _nn_backPropagation(nn_Layer* from_layer, nn_Layer* to_layer);

#endif
