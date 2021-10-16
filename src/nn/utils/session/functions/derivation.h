// derivation.h

#ifndef DERIVATION_H
#define DERIVATION_H

#include <math.h>
#include "activations.h"
#include "nn/model/layers/layer.h"
#include "nn/functions_descriptors/functions_descriptors_enums.h"


double _nn_derivativeActivation(double x, activation activation);

#endif