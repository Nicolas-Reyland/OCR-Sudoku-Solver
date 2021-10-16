// evaluate.h

#ifndef NN_EVALUATE_H
#define NN_EVALUATE_H

#include "nn/model/model.h"

void _nn_fast_forward(nn_Model* model, double* input);
void _nn_fast_forward_layer(nn_Layer* from_layer, nn_Layer* to_layer);
void _nn_backward_propagation(nn_Layer* from_layer, nn_Layer* to_layer);

#endif
