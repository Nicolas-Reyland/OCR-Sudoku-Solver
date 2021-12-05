// derivation.c
#include "derivation.h"

void _nn_dSoftmax(nn_Layer *layer);
void _nn_mapScalarFunction(nn_Layer *layer, double (*scalar_fn)(double));

double _nn_dSigmoid(double x);
double _nn_dRelu(double x);
double _nn_dLeakyRelu(double x);
double _nn_dtan_h(double x);

/* Derivative Activation */
void _nn_derivativeLayerActivation(nn_Layer *layer) {
    double (*scalar_fn)(double) = NULL;
    switch (layer->activation) {
    case SIGMOID:
        scalar_fn = &_nn_dSigmoid;
        break;
    case RELU:
        scalar_fn = &_nn_dRelu;
        break;
    case LEAKY_RELU:
        scalar_fn = &_nn_dLeakyRelu;
        break;
    case TANH:
        scalar_fn = &_nn_dtan_h;
        break;
    case SOFTMAX:
        // difficult case
        _nn_dSoftmax(layer);
        return;
    default:
        err_verbose_exit(
            "derivativeActivation: Unrecognised activation function: %s\n",
            activation_str[layer->activation]);
        return;
    }
    // scalar function application
    if (scalar_fn == NULL) {
        err_verbose_exit("Scalar function is NULL\n");
    }
    _nn_mapScalarFunction(layer, scalar_fn);
}

void _nn_dSoftmax(nn_Layer *layer) {
    // https://www.mldawn.com/back-propagation-with-cross-entropy-and-softmax/
    // https://datascience.stackexchange.com/questions/51677/derivation-of-backpropagation-for-softmax/51691#51691
    // http://saitcelebi.com/tut/output/part2.html (c'est vers le bas ->
    // control+f / F3)
    // https://medium.com/@14prakash/back-propagation-is-very-simple-who-made-it-complicated-97b794c97e5c
    // N: the number of perceptrons(nodes) in the current layer
    size_t N = layer->num_nodes;
    double *exponentials = mem_calloc(N, sizeof(double));
    // get sum^2
    double sum = 0;
    for (size_t i = 0; i < N; i++) {
        exponentials[i] = exp(layer->nodes[i]->raw_value);
        sum += exponentials[i];
    }
    double sum_squared = sum * sum;
    if (sum_squared < 1e-17 || isnan(sum)) {
        // this means something went wrong
        // better learn something (0.1) of this situation than nothing (0.0)
        for (size_t i = 0; i < N; i++)
            layer->nodes[i]->d_raw_value = 0.1;
        mem_free(exponentials);
        return;
    }
    // calculate derivative values
    for (size_t i = 0; i < N; i++) {
        layer->nodes[i]->d_raw_value =
            exponentials[i] * (sum - exponentials[i]) / sum_squared;
        if (isnan(layer->nodes[i]->d_raw_value)) {
            verbose("isnan in dsoftmax: exp[%lu] = %lf, sum = %lf, sum_squared "
                    "= %lf",
                    i, exponentials[i], sum, sum_squared);
        }
    }
    mem_free(exponentials);
}

void _nn_mapScalarFunction(nn_Layer *layer, double (*scalar_fn)(double)) {
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->d_raw_value = scalar_fn(layer->nodes[i]->raw_value);
}

double _nn_dSigmoid(double x) {
    // the 'x' is already the output of a sigmoid function
    // and we don't want to calculate d_sigmoid(sigmoid(x)), but
    // d_sigmoid(x), thus returning ' x * (1.0 - x)'
    return _nn_sigmoid(x) * (1.0 - _nn_sigmoid(x));
}

double _nn_dRelu(double x) { return x < 0 ? 0 : 1; }

double _nn_dLeakyRelu(double x) { return x < 0 ? 0 : LEAKY_RELU_VALUE; }

double _nn_dtan_h(double x) {
    double y = tanh(x);
    return 1 - y * y;
}
