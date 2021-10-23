// test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main(int argc, char** argv)
{
	setVerbose(true);
	if (argc > 1 && argv[1][0]) {
		setVerbose(false);
	}

	// init random
	initRandom();
	initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		{ .dims = 1, .x = 2, .y = 1, .z = 1 },
		{ .dims = 1, .x = 2, .y = 1, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 }
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
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);
	nn_ShapeDescription shape = { .dims = 0, .x = 0, .y = 0, .z = 0 };
	nn_Data* data_train = nn_DataLoadRaw("datas/xor/input.txt", "datas/xor/output.txt", &shape);
	nn_Data* data_test = nn_DataLoadRaw("datas/xor/input.txt", "datas/xor/output.txt", &shape);
	nn_DataSet* dataset = _nn_createDataSet(data_train, data_test);
	nn_Session* session = createSession(
		dataset,
		10,
		0.1,
		false,
		true,
		0.1
	);

	verbose("Session allocated");
	setVerbose(false);
	session->train(session, model);
	setVerbose(true);
	session->test(session, model);

	verbose("Model allocated.");

	// free model
	freeModel(model);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
