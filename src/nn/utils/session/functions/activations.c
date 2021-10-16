// activations.c

#include "activations.h"

void _nn_activateLayer(nn_Layer* layer, activation activation)
{
	switch (activation)
	{
		case SIGMOID:
			sigmoid(layer);
			break;
		case RELU:
			relu(layer);
			break;
		// case ...
		default:
			fprintf(stderr, "Unrecognised activation function: %d\n", activation);
			exit(EXIT_FAILURE);
			break;
	}
}

/* Sigmoid */
double _nn_sigmoid(double x)
{
	return 1 / (1 + exp(-x));
}

void sigmoid(nn_Layer* layer)
{
	for (size_t i = 0; i < layer->nb_nodes; i++)
		layer->nodes[i]->value = _nn_sigmoid(layer->nodes[i]->raw_value);
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
	for (size_t i = 0; i < layer->nb_nodes; i++)
		layer->nodes[i]->value = _nn_relu(layer->nodes[i]->raw_value);
}
