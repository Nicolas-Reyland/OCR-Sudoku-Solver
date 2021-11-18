// losses.c

#include "losses.h"

double _nn_binaryCrossEntropy(nn_Layer* layer, double* desired_output);
double _nn_categoricalCrossEntropy(nn_Layer* layer, double* desired_output);
double _nn_meanSquaredError(nn_Layer* layer, double* desired_output);

double applyLosses(nn_Layer* layer, double *desired_output, losses losses)
{
	switch(losses)
	{
		case NO_LOSS:
			fprintf(stderr, "Tried to evaluate empty loss function (no loss defined)\n");
			exit(EXIT_FAILURE);
			return NAN;
		case CATEGORICALCROSSENTROPY:
			return _nn_categoricalCrossEntropy(layer, desired_output);
		case BINARYCROSSENTROPY:
			return _nn_binaryCrossEntropy(layer, desired_output);
		case MEANSQUAREDERROR:
			return _nn_meanSquaredError(layer,desired_output);
		default:
			fprintf(stderr, "ApplyLosses: unrecognized losses function: %d\n",
			 losses);
			exit(EXIT_FAILURE);
			break;

	}
}

double _nn_binaryCrossEntropy(nn_Layer* layer, double* desired_output)
{
	double sum = 0;
	for(size_t i = 0; i < layer->num_nodes; i++)
	{
		sum += desired_output[i] * log(layer->nodes[i]->value) +
		(1.0 - desired_output[i])* log(1 - layer->nodes[i]->value);
	}
	sum = sum * (1/layer->num_nodes);
	return - sum;
}
/*
* Apply categoricalCrossEntropy to output Layer
*/
double _nn_categoricalCrossEntropy(nn_Layer* layer, double* desired_output)
{
	double sum = 0;
	for(size_t i = 0; i < layer->num_nodes; i++)
	{
		sum += desired_output[i] * log(layer->nodes[i]->value);
	}
	return -sum;
}

double _nn_meanSquaredError(nn_Layer* layer, double* desired_output)
{
	double sum = 0;
	for(size_t i = 0; i < layer->num_nodes; i++)
	{
		sum += (desired_output[i] - layer->nodes[i]->value) * (desired_output[i] - layer->nodes[i]->value);
	}
	double mean_square_error = (1.0 / layer->num_nodes) * sum;
	return mean_square_error;
}

