// activations.c

#include "activations.h"
double _nn_sigmoid(double x);
double _nn_relu(double x);
double _nn_softmax(double sum, double x);

void _nn_activateLayer(nn_Layer* layer)
{
	switch (layer->activation)
	{
		case SIGMOID:
			sigmoid(layer);
			break;
		case RELU:
			relu(layer);
			break;
		case SOFTMAX:
			softmax(layer);
			break;
		case NO_ACTIVATION:
			fprintf(stderr, "Tried to evaluate empty activation function (no activation defined)\n");
			exit(EXIT_FAILURE);
			break;
		default:
			fprintf(stderr, "Unrecognised activation function: %d\n", layer->activation);
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
	for (size_t i = 0; i < layer->num_nodes; i++)
		layer->nodes[i]->value = _nn_sigmoid(layer->nodes[i]->raw_value);
}

/* ReLu */
double _nn_relu(double x)
{
	return x > 0 ? x : 0;
}

void relu(nn_Layer* layer)
{
	for (size_t i = 0; i < layer->num_nodes; i++)
		layer->nodes[i]->value = _nn_relu(layer->nodes[i]->raw_value);
}

double _nn_softmax(double sum, double x)
{
	return sum / x;
}

/* SoftMAX function applied to whole layer*/
void softmax(nn_Layer* layer)
{
	double sum =0;
	for(size_t i = 0; i < layer->num_nodes;i++)
		sum += exp(layer->nodes[i]->raw_value);

	for(size_t i = 0; i< layer->num_nodes;i++)
		layer->nodes[i]->value = _nn_softmax(sum, layer->nodes[i]->raw_value);
}
