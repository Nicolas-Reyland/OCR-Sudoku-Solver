// activations.h

#ifndef NN_ACTIVATIONS_H
#define NN_ACTIVATIONS_H

#include <math.h>
#include "nn/model/layers/layer.h"

/* We must have a void return type and take a layer as input
 * because of the softmax activation function
 */

void sigmoid(nn_Layer* layer);
void relu(nn_Layer* layer);
void leaky_relu(nn_Layer* layer);
void softmax(nn_Layer* layer);
void tan_h(nn_Layer* layer);

#endif
