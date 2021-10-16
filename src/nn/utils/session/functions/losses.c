// losses.c

#include "losses.h"

double _nn_binaryCrossEntropy(nn_Layer* layer, double* desired_output);
double _nn_categoricalCrossEntropy(nn_Layer* layer, double* desired_output);
double _nn_sparseCategoricalCrossEntropy(nn_Layer* layer, 
double *desired_output);

double applyLosses(nn_Layer* layer, double *desired_output, losses losses)
{
	switch(losses)
	{
		case CATEGORICALCROSSENTROPY:
			return _nn_categoricalCrossEntropy(layer,desired_output);
			break;
		case SPARSECATEGORICALCROSSENTROPY:
			return _nn_sparseCategoricalCrossEntropy(layer,desired_output);
			break;
		case BINARYCROSSENTROPY:
			return _nn_binaryCrossEntropy(layer,desired_output);
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
	for(size_t i = 0; i < layer->nb_nodes; i++)
	{
		sum += desired_output[i] * log(layer->nodes[i]->value) + 
		(1 - desired_output[i])* log(1 - layer->nodes[i]->value);
	}
	sum = sum * (1/layer->nb_nodes);
	return - sum;
}
/*
* Apply categoricalCrossEntropy to output Layer
*/
double _nn_categoricalCrossEntropy(nn_Layer* layer, double* desired_output)
{
	double sum = 0;
	for(size_t i = 0; i < layer->nb_nodes; i++)
	{
		sum += desired_output[i] * log(layer->nodes[i]->value);
	}
	return -sum;
}

double _nn_sparseCategoricalCrossEntropy(nn_Layer* layer, 
double* desired_output)
{
	for(size_t i = 0; i < layer->nb_nodes; i++)
	{
		
	}
}