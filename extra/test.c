// test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main(int argc, char** argv)
{
	setVerbose(true);

	// init random
	initRandom();
	initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[4] = {
		create2DShapeDescription(28, 28),
		create2DShapeDescription(16, 16),
		create1DShapeDescription(128),
		create1DShapeDescription(9),
	};
	// activation functions
	activation activations[3] = {
		RELU, RELU, SOFTMAX
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	verbose("Allocating model...");
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);
	verbose("Model allocated");
	nn_ShapeDescription shape = emptyShapeDescription();

	nn_DataSet* dataset = nn_loadDataSet("datas/mnist/", &shape, true);

	nn_Session* session = createSession(
		dataset,
		10,
		0.1,
		false,
		false,
		0.1,
		NULL,
		NULL
	);

	verbose("Session allocated");

	setVerbose(false);
	session->train(session, model);
	session->test(session, model);

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
