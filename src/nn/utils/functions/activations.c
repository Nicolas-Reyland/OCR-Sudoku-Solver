// activations.c

#include "activations.h"

/* Sigmoid */
double _nn_sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

void sigmoid(nn_Layer* layer)
{
}

/* ReLu */
double _nn_relu(double x)
{
	if (x < 0) {
		return 0;
	}
	return x > 1 ? 1 : x;
}

void relu(nn_Layer* layer)
{
}

