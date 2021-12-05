// evaluate.h

#ifndef NN_EVALUATE_H
#define NN_EVALUATE_H

#include "nn/model/model.h"
#include "nn/utils/session/functions/activations.h"
#include "nn/utils/session/functions/derivation.h"
#include "nn/utils/session/functions/losses.h"

void _nn_feedForward(nn_Model *model, double *input);
void _nn_backPropagation(nn_Model *model, double *desired_output);
void _nn_updateWeights(nn_Model *model, double learning_rate);

#endif
