// test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

#define NUM_LAYERS 3

extern linked_list* GPL;

int main(int argc, char** argv)
{
	if (argc > 1 && argv[1][0]) {
		setVerbose(false);
	}

	// init random
	initRandom();
	initMemoryTracking();
	// model architecture
	nn_ShapeDescription model_architecture[NUM_LAYERS] = {
		createShapeDescription(3, 3, 3), // { .dims = 3, .x = 3, .y = 3, .z = 3 },
		create2DShapeDescription(5, 2), // { .dims = 2, .x = 5, .y = 2, .z = 1 },
		create1DShapeDescription(1), // { .dims = 1, .x = 1, .y = 1, .z = 1 }
	};
	// activation functions
	activation activations[2] = {
		SIGMOID, SOFTMAX
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	verbose("Allocating model...");

	// malloc model
	nn_Model* model = createModel(NUM_LAYERS, model_architecture, activations, loss, optimizer);

	verbose("Model allocated.");

	// free model
	freeModel(model);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
