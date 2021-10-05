// activations.h

#ifndef NN_ACTIVATIONS_H
#define NN_ACTIVATIONS_H

#include <math.h>

double sigmoid(double x);

double relu(double x);

double leaky_relu(double x);

double softmax(Layer* layer);

double tanh(double x);

#endif
