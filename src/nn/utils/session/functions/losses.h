// losses.h

#ifndef NN_LOSSES_H
#define NN_LOSSES_H

#include "nn/model/layers/layer.h"

double* applyLosses(nn_Layer* layer, double *desired_output, losses losses);



#endif
