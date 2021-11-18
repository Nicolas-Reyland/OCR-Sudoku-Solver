#include "use_model.h"

double* _nn_useModel(struct nn_Model* model, double* input)
{
	double* output = mem_malloc(sizeof(double)* model->layers[model->num_layers -1]->num_nodes);

	_nn_feedForward(model,input);

	for(size_t i = 0; i < model->layers[model->num_layers -1]->num_nodes; i++)
	{
		output[i] = model->layers[model->num_layers -1]->nodes[i]->value;
	}

	return output;
}