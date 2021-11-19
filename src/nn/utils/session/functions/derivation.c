// derivation.c
#include "derivation.h"


double dSigmoid(double x);
double dRelu(double x);
double dLeakyRelu(double x);
double dtan_h(double x);
void dSoftmax(nn_Layer* layer);

void _nn_derivativeLayerActivation(nn_Layer* layer)
{
    //double (monoid*)(double);
    switch(layer->activation)
    {
        case SIGMOID:
            for (size_t i = 0; i < layer->num_nodes; i++)
                layer->nodes[i]->d_raw_value = dSigmoid(layer->nodes[i]->raw_value);
            break;
        case RELU:
            for (size_t i = 0; i < layer->num_nodes; i++)
                layer->nodes[i]->d_raw_value = dRelu(layer->nodes[i]->raw_value);
            break;
        case LEAKY_RELU:
            for (size_t i = 0; i < layer->num_nodes; i++)
                layer->nodes[i]->d_raw_value = dLeakyRelu(layer->nodes[i]->raw_value);
            break;
        case TANH:
            for (size_t i = 0; i < layer->num_nodes; i++)
                layer->nodes[i]->d_raw_value = dtan_h(layer->nodes[i]->raw_value);
            break;
        case SOFTMAX:
            // difficult case
            dSoftmax(layer);
            break;
        default:
            fprintf(stderr,
                "derivativeActivation: Unrecognised activation function: %s\n",
                activation_str[layer->activation]);
                exit(EXIT_FAILURE);
            break;
    }
}

/* Derivative Activation */
double _nn_derivativeActivation(double x, activation activation)
{
    return 1.0;

    // TODO: remove this function
    double output;
    switch(activation)
    {
        case SIGMOID:
            return dSigmoid(x);
        case RELU:
            return dRelu(x);
        case LEAKY_RELU:
            return dLeakyRelu(x);
        case TANH:
            return dtan_h(x);
        case SOFTMAX:
            // returning 1.0 so it has no impact (x * derivative = x * 1.0 = x)
            return 1.0;
        default:
            fprintf(stderr,
                "derivativeActivation: Unrecognised activation function: %d\n",
                activation);
                exit(EXIT_FAILURE);
            break;
    }
    return output;
}

void dSoftmax(nn_Layer* layer)
{
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->d_raw_value = 1.0;
}

double dSigmoid(double x)
{
    // the 'x' is already the output of a sigmoid function
    // and we don't want to calculate d_sigmoid(sigmoid(x)), but
    // d_sigmoid(x), thus returning ' x * (1.0 - x)'
    return _nn_sigmoid(x) * (1.0 - _nn_sigmoid(x));
}

double dRelu(double x)
{
    return x < 0 ? 0 : 1;
}

double dLeakyRelu(double x)
{
    return x < 0 ? 0 : LEAKY_RELU_VALUE;
}

double dtan_h(double x)
{
    double y = tanh(x);
    return 1 - y * y;
}

