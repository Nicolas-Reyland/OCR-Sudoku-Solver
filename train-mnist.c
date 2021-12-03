// test-train-save.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main()
{
	setVerbose(true);

	// init random
	nn_initRandom();
	nn_initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[] = {
		nn_create2DShapeDescription(28, 28),
		nn_create2DShapeDescription(16, 16),
		nn_create1DShapeDescription(128),
		nn_create1DShapeDescription(9),
	};
	// activation functions
	activation activations[] = {
		TANH, SIGMOID, SOFTMAX
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	verbose("Allocating model...");
	// malloc model
	nn_Model* model = nn_createModel(4, model_architecture, activations, loss, optimizer);
	verbose("Model allocated");
    model->printModelArchitecture(model);

	// load the dataset
	nn_ShapeDescription shape = nn_emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/mnist/1k-", &shape, true);

	nn_Session* session = nn_createSession(
		dataset,
		40,
		0.1,
		false,
		true,
		0.1,
		NULL,
		NULL
	);

	verbose("Session allocated");

	session->train(session, model);
	session->test(session, model);

    model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	verbose("Saving the model...");
	model->saveModel(model, "save/mnist/1k-");

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
