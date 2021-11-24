// derivation.c
#include "derivation.h"


void _nn_dSoftmax(nn_Layer* layer);
void _nn_mapScalarFunction(nn_Layer* layer, double (*scalar_fn)(double));

double _nn_dSigmoid(double x);
double _nn_dRelu(double x);
double _nn_dLeakyRelu(double x);
double _nn_dtan_h(double x);

/* Derivative Activation */
void _nn_derivativeLayerActivation(nn_Layer* layer)
{
    double (*scalar_fn)(double) = NULL;
    switch(layer->activation)
    {
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
            fprintf(stderr,
                "derivativeActivation: Unrecognised activation function: %s\n",
                activation_str[layer->activation]);
            exit(EXIT_FAILURE);
            return;
    }
    // scalar function application
    if (scalar_fn == NULL) {
        fprintf(stderr, "Scalar function is NULL\n");
        exit(EXIT_FAILURE);
    }
    _nn_mapScalarFunction(layer, scalar_fn);
}

void _nn_dSoftmax(nn_Layer* layer)
{
    // https://www.mldawn.com/back-propagation-with-cross-entropy-and-softmax/
    // https://datascience.stackexchange.com/questions/51677/derivation-of-backpropagation-for-softmax/51691#51691
    // http://saitcelebi.com/tut/output/part2.html (c'est vers le bas -> control+f / F3)
    /* Il est possible qu'on doive recoder une partie de la backpropagation ...
     */
    // N: the number of perceptrons(nodes) in the current layer
    size_t N = layer->num_nodes;
    // reset the layer nodes d_raw_value's to zero
    for (size_t i = 0; i < N; i++)
        layer->nodes[i]->d_raw_value = 0;
    // allocate memory on the heap for the jacobian matrix associated with the softmax function
    double* jacobian_matrix = malloc(N * N * sizeof(double));
    // fill the jacobian matrix
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            if (i == j)
                jacobian_matrix[i * N + j] = layer->nodes[i]->value * (1 - layer->nodes[i]->value);
            else
                jacobian_matrix[i * N + j] = -layer->nodes[i]->value * layer->nodes[j]->value;
    // multiply the jacobian matrix of softmax to the layer nodes :
    // the layer is treated as a vector, and it's nodes are treated as
    // the scalars composing this vector
    // So say J is the jacobian matrix and u is the vector associated to the layer,
    // then 'v = J x u' and 'x' is the matrix multiplication sign
    for (size_t i = 0; i < N /* R1 */; i++) {
        for (size_t j = 0; j < N /* R2 */; j++) {
            //verbose("jacobian value: %lf", jacobian_matrix[N * j + i]);
            layer->nodes[j]->d_raw_value += jacobian_matrix[N * j + i] * layer->nodes[i]->value;
        }
    }

    // for (size_t i = 0; i < layer->num_nodes; i++) {
    //     double a_i = layer->nodes[i]->raw_value;
    //     layer->nodes[i]->d_raw_value = a_i * (1 - a_i);
    //     if (isnan(layer->nodes[i]->d_raw_value)) {
    //         verbose("isnan: %lf | %lf", layer->nodes[i]->d_raw_value, a_i);
    //         exit(EXIT_FAILURE);
    //     }
    // }
}


void _nn_mapScalarFunction(nn_Layer* layer, double (*scalar_fn)(double))
{
    for (size_t i = 0; i < layer->num_nodes; i++)
        layer->nodes[i]->d_raw_value = scalar_fn(layer->nodes[i]->raw_value);
}

double _nn_dSigmoid(double x)
{
    // the 'x' is already the output of a sigmoid function
    // and we don't want to calculate d_sigmoid(sigmoid(x)), but
    // d_sigmoid(x), thus returning ' x * (1.0 - x)'
    return _nn_sigmoid(x) * (1.0 - _nn_sigmoid(x));
}

double _nn_dRelu(double x)
{
    return x < 0 ? 0 : 1;
}

double _nn_dLeakyRelu(double x)
{
    return x < 0 ? 0 : LEAKY_RELU_VALUE;
}

double _nn_dtan_h(double x)
{
    double y = tanh(x);
    return 1 - y * y;
}

