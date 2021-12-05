// losses.c

#include "losses.h"

double _nn_binaryCrossEntropy(nn_Layer *layer, double *desired_output);
double _nn_categoricalCrossEntropy(nn_Layer *layer, double *desired_output);
double _nn_meanSquaredError(nn_Layer *layer, double *desired_output);

double applyLosses(nn_Layer *layer, double *desired_output, losses loss) {
    switch (loss) {
    case NO_LOSS:
        err_verbose_exit(
            "Tried to evaluate empty loss function (no loss defined)\n");
        return NAN;
    case CATEGORICALCROSSENTROPY:
        return _nn_categoricalCrossEntropy(layer, desired_output);
    case BINARYCROSSENTROPY:
        return _nn_binaryCrossEntropy(layer, desired_output);
    case MEANSQUAREDERROR:
        return _nn_meanSquaredError(layer, desired_output);
    default:
        break;
    }
    err_verbose_exit("ApplyLosses: unrecognized losses function: %s\n",
                     losses_str[loss]);
    return NAN;
}

double _nn_binaryCrossEntropy(nn_Layer *layer, double *desired_output) {
    double sum = 0;
    for (size_t i = 0; i < layer->num_nodes; i++) {
        sum += desired_output[i] * log(layer->nodes[i]->value) +
               (1.0 - desired_output[i]) * log(1 - layer->nodes[i]->value);
    }
    sum = sum * (1 / layer->num_nodes);
    return -sum;
}

/*
 * Apply categoricalCrossEntropy to output Layer
 */
double _nn_categoricalCrossEntropy(nn_Layer *layer, double *desired_output) {
    double sum = 0;
    for (size_t i = 0; i < layer->num_nodes; i++) {
        /*if (layer->nodes[i]->value <= 0) {
                printf("Value: %lf\n", layer->nodes[i]->value);
        }*/
        if (layer->nodes[i]->value == 0) {
            if (desired_output[i] == 0.0) {
                // the desired output is zero, so it's 0 * log(x)
                // thus, we can simply say that it's zero. And go on with our
                // loss calculation
                continue;
            } else {
                // the desired output is (somewhat) 'hot'
                sum += -INFINITY;
            }
        } else if (layer->nodes[i]->value < 0) {
            err_verbose_exit("FATAL ERROR CCE: injected value in log cannot be "
                             "negative or zero: %lf",
                             layer->nodes[i]->value);
        }
        sum += desired_output[i] * log(layer->nodes[i]->value);
    }
    return -sum;
}

double _nn_meanSquaredError(nn_Layer *layer, double *desired_output) {
    double sum = 0;
    for (size_t i = 0; i < layer->num_nodes; i++) {
        sum += (desired_output[i] - layer->nodes[i]->value) *
               (desired_output[i] - layer->nodes[i]->value);
    }
    double mean_square_error = (1.0 / layer->num_nodes) * sum;
    return mean_square_error;
}
