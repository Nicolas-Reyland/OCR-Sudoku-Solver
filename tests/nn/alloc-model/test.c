// test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

#define NUM_LAYERS 3

int main(int argc, char **argv) {
    if (argc > 1 && argv[1][0]) {
        setVerbose(false);
    }

    PROGRESS_BAR_STATUS = false;

    // init random
    nn_initRandom();
    initMemoryTracking();
    // model architecture
    nn_ShapeDescription model_architecture[NUM_LAYERS] = {
        nn_createShapeDescription(3, 3,
                                  3), // { .dims = 3, .x = 3, .y = 3, .z = 3 },
        nn_create2DShapeDescription(
            5, 2), // { .dims = 2, .x = 5, .y = 2, .z = 1 },
        nn_create1DShapeDescription(1), // { .dims = 1, .x = 1, .y = 1, .z = 1 }
    };
    // activation functions
    activation activations[2] = {SIGMOID, SOFTMAX};
    // loss & optimizers
    losses loss = CATEGORICALCROSSENTROPY;
    optimizer optimizer = ADAM;

    verbose("Allocating model...");

    // malloc model
    nn_Model *model = nn_createModel(NUM_LAYERS, model_architecture,
                                     activations, loss, optimizer);

    verbose("Model allocated.");

    // free model
    nn_freeModel(model);

    verbose("Model freed.");

    mem_freeGPL(false);

    return 0;
}
