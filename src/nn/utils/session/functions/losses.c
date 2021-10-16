// losses.c

#include "losses.h"

void _nn_binaryCrossEntropy(nn_Layer* layer, double *desired_output);
void _nn_categoricalCrossEntropy(nn_Layer* layer, double *desired_output);
void _nn_sparseCategoricalCrossEntropy(nn_Layer* layer, 
double *desired_output);

void applyLosses(nn_Layer* layer, double *desired_output, losses losses)
{
	switch(losses)
	{
		case CATEGORICALCROSSENTROPY:
			break;
		case SPARSECATEGORICALCROSSENTROPY:
			break;
		case BINARYCROSSENTROPY:
			fprintf(stderr, "ApplyLosses: unrecognized losses function: %d\n",
			 losses);
			exit(EXIT_FAILURE);
			break;
		default:
			
	}
}