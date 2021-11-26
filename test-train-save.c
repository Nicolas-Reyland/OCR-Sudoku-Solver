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
		RELU, SIGMOID, SOFTMAX
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
	nn_DataSet* dataset = nn_loadDataSet("datas/mnist/", &shape, true);

	nn_Session* session = createSession(
		dataset,
		15,
		0.1,
		false,
		true,
		0.05
	);

	verbose("Session created");

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	setVerbose(true);
	verbose("Saving the model...");
	model->saveModel(model, "save/mnist/");

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
