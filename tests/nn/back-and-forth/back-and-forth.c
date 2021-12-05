// back-and-forth.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list *GPL;
extern bool _nn_random_init_done;

void print_neural_network(nn_Model *model) {
    size_t num_layers = model->num_layers;
    nn_Layer **layers = model->layers;
    size_t num_neurons[3] = {
        layers[0]->num_nodes,
        layers[1]->num_nodes,
        layers[2]->num_nodes,
    };
    char fstr_w[] = "\tlayers[%ld]->nodes->[%ld]->weights[%ld] = %lf\n";
    char fstr_dw[] = "\tlayers[%ld]->nodes->[%ld]->d_weights[%ld] = %lf\n";
    char fstr_db[] = "\tlayers[%ld]->nodes->[%ld]->bias = %lf\n";
    char fstr_b[] = "\tlayers[%ld]->nodes->[%ld]->bias = %lf\n";
    char fstr_rv[] = "\tlayers[%ld]->nodes->[%ld]->raw_value = %lf\n";
    char fstr_av[] = "\tlayers[%ld]->nodes->[%ld]->value = %lf\n";
    char fstr_drv[] = "\tlayers[%ld]->nodes->[%ld]->d_raw_value = %lf\n";
    char fstr_dav[] = "\tlayers[%ld]->nodes->[%ld]->d_value = %lf\n";
    printf(" ------------------------------------------------------------- \n");
    for (size_t i = 0; i < num_layers; i++) {
        printf("layer : %ld\n", i);
        for (size_t j = 0; j < num_neurons[i]; j++) {
            // value, dvaue, etc.
            printf(fstr_rv, i, j, layers[i]->nodes[j]->raw_value);
            printf(fstr_av, i, j, layers[i]->nodes[j]->value);
            printf(fstr_drv, i, j, layers[i]->nodes[j]->d_raw_value);
            printf(fstr_dav, i, j, layers[i]->nodes[j]->d_value);
            if (i != num_layers - 1) {
                // weights, dweights
                for (size_t k = 0; k < num_neurons[i + 1]; k++) {
                    printf(fstr_w, i, j, k, layers[i]->nodes[j]->weights[k]);
                    printf(fstr_dw, i, j, k, layers[i]->nodes[j]->d_weights[k]);
                }
            }
            // print bias
            if (i != 0) {
                printf(fstr_b, i, j, layers[i]->nodes[j]->bias);
                printf(fstr_db, i, j, layers[i]->nodes[j]->d_bias);
            }
            printf("\n");
        }
    }
}

void oneLearningStep(nn_Model *model, double *input, double *output,
                     double learning_rate, bool verb) {
    if (verb)
        printf("base\n");
    if (verb)
        print_neural_network(model);
    _nn_feedForward(model, input);

    if (verb)
        printf("forward prop\n");
    if (verb)
        print_neural_network(model);
    _nn_backPropagation(model, output);

    if (verb)
        printf("backward prop\n");
    if (verb)
        print_neural_network(model);
    _nn_updateWeights(model, learning_rate);

    if (verb)
        printf("weights updated\n");
    if (verb)
        print_neural_network(model);
}

int main(int argc, char **argv) {
    setVerbose(false);
    // init random
    // nn_initRandom(); // NOT initializing random this ways
    // custom random init
    nn_initRandom(); // overwriting weights anyway
    initMemoryTracking();

    // model architecture
    nn_ShapeDescription model_architecture[3] = {
        nn_create1DShapeDescription(2),
        nn_create1DShapeDescription(2),
        nn_create1DShapeDescription(1),
    };

    // activation functions
    activation activations[2] = {RELU, SIGMOID};
    // loss & optimizers
    losses loss = MEANSQUAREDERROR;
    optimizer optimizer = ADAM;
    // malloc model
    nn_Model *model =
        nn_createModel(3, model_architecture, activations, loss, optimizer);

    double weights[3][2] = {
        {0.840188, 0.394383},
        {0.783099, 0.798440},
        {0.911647, 0.335223},
    };

    double bias[3] = {
        0.197551,
        0.768230,
        0.277775,
    };

    // Initialize the weights
    model->layers[0]->nodes[0]->weights[0] = weights[0][0];
    model->layers[0]->nodes[0]->weights[1] = weights[0][1];
    model->layers[0]->nodes[1]->weights[0] = weights[1][0];
    model->layers[0]->nodes[1]->weights[1] = weights[1][1];
    model->layers[1]->nodes[0]->weights[0] = weights[2][0];
    model->layers[1]->nodes[1]->weights[0] = weights[2][1];

    // Initialize the bias
    model->layers[1]->nodes[0]->bias = bias[0];
    model->layers[1]->nodes[1]->bias = bias[1];
    model->layers[2]->nodes[0]->bias = bias[2];

    // one manual forward/backward propagation
    double input_collection[4][2] = {
        {1.0, 1.0},
        {1.0, 0.0},
        {0.0, 1.0},
        {0.0, 0.0},
    };
    double output_collection[4][1] = {
        {0.0},
        {1.0},
        {1.0},
        {0.0},
    };

    for (int i = 0; i < 20000; i++) {
        for (int j = 0; j < 4; j++) {
            double *input = input_collection[j];
            double *output = output_collection[j];
            oneLearningStep(model, input, output, 0.15, false);
        }
    }
    for (int j = 0; j < 4; j++) {
        double *input = input_collection[j];
        double *output = output_collection[j];
        oneLearningStep(model, input, output, 0.15, true);
    }

    // free model
    nn_freeModel(model);
    // free session (and dataset)
    // nn_freeSession(session);
    mem_freeGPL(false);

    return 0;
}
