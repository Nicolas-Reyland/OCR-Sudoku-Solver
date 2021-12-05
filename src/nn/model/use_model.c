#include "use_model.h"

double *_nn_useModel(nn_Model *model, double *input) {
    double *output = mem_malloc(
        sizeof(double) * model->layers[model->num_layers - 1]->num_nodes);

    _nn_feedForward(model, input);
    nn_Layer *output_layer = model->layers[model->num_layers - 1];

    for (size_t i = 0; i < output_layer->num_nodes; i++)
        output[i] = output_layer->nodes[i]->value;

    return output;
}