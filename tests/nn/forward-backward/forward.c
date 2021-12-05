// forward.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list *GPL;
extern bool _nn_random_init_done;

int main() {
    setVerbose(false);

    // init random
    // nn_initRandom(); // NOT initializing random this ways
    // custom random init
    srand(1234567890);
    _nn_random_init_done = true;
    printf("Next random integer is: %d\n", rand());
    initMemoryTracking();
    // model architecture
    nn_ShapeDescription model_architecture[3] = {
        nn_create1DShapeDescription(3),
        nn_create1DShapeDescription(4),
        nn_create1DShapeDescription(2),
    };
    // activation functions
    activation activations[3] = {RELU, SIGMOID};
    // loss & optimizers
    losses loss = CATEGORICALCROSSENTROPY;
    optimizer optimizer = ADAM;
    // malloc model
    nn_Model *model =
        nn_createModel(3, model_architecture, activations, loss, optimizer);

    // print the layers weights
    model->printModelLayers(model);

    // feed input data to the model
    double input_data[3] = {0.1, -0.23, 0.75};
    _nn_feedForward(model, input_data);
    printf("forward feeding done\n");

    // print the layers values
    // model->printModelLayers(model);
    model->printModelLayersValues(model);

    // free model
    nn_freeModel(model);
    mem_freeGPL(false);

    return 0;
}
