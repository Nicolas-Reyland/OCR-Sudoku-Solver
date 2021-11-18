// derivation.c
#include "derivation.h"


double dSigmoid(double x);
double dRelu(double x);
double dLeakyRelu(double x);
double dtan_h(double x);

/* Derivative Activation */
double _nn_derivativeActivation(double x, activation activation)
{
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
            // there is no derivative for softmax
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

