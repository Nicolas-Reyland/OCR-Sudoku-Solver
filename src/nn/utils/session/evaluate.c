// evaluate .c

#include "evaluate.h"

void _nn_feedForward(nn_Model *model, double *input) {
    nn_Layer **const layers = model->layers;
    size_t num_layers = model->num_layers;

    // feed input layer with input
    for (size_t i = 0; i < layers[0]->num_nodes; i++) {
        layers[0]->nodes[i]->value = input[i];
    }

    // forward propagation to all the other layers (hidden & output)
    for (size_t i = 1; i < num_layers; i++) {
        for (size_t j = 0; j < layers[i]->num_nodes; j++) {
            layers[i]->nodes[j]->raw_value = layers[i]->nodes[j]->bias;
            for (size_t k = 0; k < layers[i - 1]->num_nodes; k++) {
                double local_raw_value = layers[i - 1]->nodes[k]->weights[j] *
                                         layers[i - 1]->nodes[k]->value;
                layers[i]->nodes[j]->raw_value += local_raw_value;
            }
        }
        _nn_activateLayer(layers[i]);
    }
}

void _nn_backPropagation(nn_Model *model, double *desired_output) {
    // https://machinelearningmastery.com/implement-backpropagation-algorithm-scratch-python/
    // TODO: update comments

    // get model layers struct
    nn_Layer **const layers = model->layers;
    size_t num_layers = model->num_layers;

    // output layer error rates
    _nn_derivativeLayerActivation(layers[num_layers - 1]);
    for (size_t j = 0; j < layers[num_layers - 1]->num_nodes; j++) {
        //
        double d_error =
            layers[num_layers - 1]->nodes[j]->value - desired_output[j];
        layers[num_layers - 1]->nodes[j]->d_raw_value *=
            d_error; // * _nn_derivativeActivation(layers[num_layers-1]->nodes[j]->raw_value, layers[num_layers-1]->activation);
        //
        for (size_t k = 0; k < layers[num_layers - 2]->num_nodes; k++) {
            layers[num_layers - 2]->nodes[k]->d_weights[j] =
                layers[num_layers - 1]->nodes[j]->d_raw_value *
                layers[num_layers - 2]->nodes[k]->value;
            layers[num_layers - 2]->nodes[k]->d_value =
                layers[num_layers - 2]->nodes[k]->weights[j] *
                layers[num_layers - 1]->nodes[j]->d_raw_value;
        }

        layers[num_layers - 1]->nodes[j]->d_bias =
            layers[num_layers - 1]->nodes[j]->d_raw_value;
    }

    // hidden & input layers
    for (size_t i = num_layers - 2; i > 0; i--) {
        _nn_derivativeLayerActivation(layers[i]);
        for (size_t j = 0; j < layers[i]->num_nodes; j++) {
            if (isnan(layers[i]->nodes[j]->d_raw_value)) {
                err_verbose_exit("d_raw_value is nan");
            }
            layers[i]->nodes[j]->d_raw_value *=
                layers[i]
                    ->nodes[j]
                    ->d_value; // * _nn_derivativeActivation(layers[i]->nodes[j]->raw_value, layers[i]->activation);

            for (size_t k = 0; k < layers[i - 1]->num_nodes; k++) {
                layers[i - 1]->nodes[k]->d_weights[j] =
                    layers[i]->nodes[j]->d_raw_value *
                    layers[i - 1]->nodes[k]->value;
                if (i > 1)
                    layers[i - 1]->nodes[k]->d_value =
                        layers[i - 1]->nodes[k]->weights[j] *
                        layers[i]->nodes[j]->d_raw_value;
            }

            layers[i]->nodes[j]->d_bias = layers[i]->nodes[j]->d_raw_value;
        }
    }
}

void _nn_updateWeights(nn_Model *model, double learning_rate) {
    size_t num_layers = model->num_layers;
    nn_Layer **const layers = model->layers;

    for (size_t i = 0; i < num_layers - 1; i++) {
        for (size_t j = 0; j < layers[i]->num_nodes; j++) {
            for (size_t k = 0; k < layers[i + 1]->num_nodes; k++) {
                layers[i]->nodes[j]->weights[k] -=
                    learning_rate * layers[i]->nodes[j]->d_weights[k];
            }
            layers[i]->nodes[j]->bias -=
                learning_rate * layers[i]->nodes[j]->d_bias;
        }
    }
}
