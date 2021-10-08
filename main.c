#include <stdio.h>
#include "src/nn/nn.h"
#include "src/nn/model/model.h"
#include "src/nn/model/model.h"
#include "src/nn/utils/misc/randomness.h"

int main()
{
	// init random
	_nn_initRandom();
	// model architecture
	ShapeDescription model_architecture[3] = {
		{ .dims = 3, .x = 3, .y = 3, .z = 3 },
		{ .dims = 2, .x = 5, .y = 2, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 }
	};
	// activation functions
	activationFunction activations[3] = {
		0, 1, 2
	};
	// loss & optimizers
	lossFunction loss = 0;
	optimizer optimizer = 0;
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);

	return 0;
}
