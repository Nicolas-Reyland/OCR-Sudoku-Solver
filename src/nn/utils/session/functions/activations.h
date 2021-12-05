// activations.h

#ifndef NN_ACTIVATIONS_H
#define NN_ACTIVATIONS_H

#define LEAKY_RELU_VALUE 0.01

#include "nn/model/layers/layer.h"
#include "utils/verbosity/verbose.h"
#include <math.h>

/* We must have a void return type and take a layer as input
 * because of the softmax activation function
 */

void _nn_activateLayer(nn_Layer *layer);

void sigmoid(nn_Layer *layer);
double _nn_sigmoid(double x);

void relu(nn_Layer *layer);
double _nn_relu(double x);

void leaky_relu(nn_Layer *layer);
double _nn_leaky_relu(double x);

void tan_h(nn_Layer *layer);
double _nn_tan_h(double x);

void softmax(nn_Layer *layer);

#endif
