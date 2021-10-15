#include "utils/verbosity/verbose.h"
#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

int main(int argc, char** argv)
{
	if (argc > 1 && argv[1][0]) {
		setVerbose(false);
	}

	verbose("Allocating model...\n");
	// init random
	initRandom();
	initMemoryTracking();
	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		{ .dims = 3, .x = 3, .y = 3, .z = 3 },
		{ .dims = 2, .x = 5, .y = 2, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 }
	};
	// activation functions
	activation activations[3] = {
		0, 1, 2
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	verbose("Allocating model...\n");

	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);

	verbose("Model allocated.\n");

	// free model
	freeModel(model);

	return 0;
}
