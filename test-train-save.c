// test-train-save.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main()
{
	setVerbose(true);

	// init random
	initRandom();
	initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[] = {
		create2DShapeDescription(28, 28),
		create2DShapeDescription(16, 16),
		create1DShapeDescription(128),
		create1DShapeDescription(9),
	};
	// activation functions
	activation activations[] = {
		SIGMOID, SIGMOID, SOFTMAX
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	// malloc model
	nn_Model* model = createModel(4, model_architecture, activations, loss, optimizer);
	verbose("Model created");
	model->printModelArchitecture(model);

	// load the dataset
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/mnist/1k-", &shape, true);

	nn_Session* session = createSession(
		dataset,
		2,
		0.1,
		false,
		true,
		0.1
	);

	verbose("Session allocated");

	session->train(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	setVerbose(false);
	verbose("Saving the model...");
	model->saveModel(model, "save/mnist/1k-");

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
