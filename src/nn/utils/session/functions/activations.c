// activations.c

#include "activations.h"

void _nn_activateLayer(nn_Layer *layer) {
    switch (layer->activation) {
    case SIGMOID:
        sigmoid(layer);
        break;
    case RELU:
        relu(layer);
        break;
    case LEAKY_RELU:
        leaky_relu(layer);
        break;
    case SOFTMAX:
        softmax(layer);
        break;
    case TANH:
        tan_h(layer);
        break;
    case NO_ACTIVATION:
        err_verbose_exit("Tried to evaluate empty activation function (no "
                         "activation defined)\n");
        break;
    default:
        err_verbose_exit("Unrecognised activation function: %d\n",
                         layer->activation);
        break;
    }
}

/* Sigmoid */
double _nn_sigmoid(double x) { return 1 / (1 + exp(-x)); }

void sigmoid(nn_Layer *layer) {
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->value = _nn_sigmoid(layer->nodes[i]->raw_value);
}

/* ReLU */
double _nn_relu(double x) { return x > 0 ? x : 0; }

void relu(nn_Layer *layer) {
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->value = _nn_relu(layer->nodes[i]->raw_value);
}

/* Keaky ReLU */
double _nn_leaky_relu(double x) { return x > 0 ? LEAKY_RELU_VALUE * x : 0; }

void leaky_relu(nn_Layer *layer) {
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->value = _nn_relu(layer->nodes[i]->raw_value);
}

double _nn_softmax(double x, double sum) { return exp(x) / sum; }

/* SoftMAX function applied to whole layer*/
void softmax(nn_Layer *layer) {
    double sum = 0;
    for (size_t i = 0; i < layer->num_nodes; i++)
        sum += exp(layer->nodes[i]->raw_value);

    for (size_t i = 0; i < layer->num_nodes; i++) {
        layer->nodes[i]->value = _nn_softmax(layer->nodes[i]->raw_value, sum);
    }
}

double _nn_tan_h(double x) { return tanh(x); }

void tan_h(nn_Layer *layer) {
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->value = _nn_tan_h(layer->nodes[i]->raw_value);
}
