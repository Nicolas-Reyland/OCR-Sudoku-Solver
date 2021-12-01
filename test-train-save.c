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
		create1DShapeDescription(200),
		create1DShapeDescription(9),
	};
	const size_t num_layers = sizeof(model_architecture) / sizeof(nn_ShapeDescription);
	const size_t num_activations = num_layers - 1;
	// activation functions
	activation activations[] = {
		SIGMOID, SOFTMAX
	};
	if (sizeof(activations) != num_activations * sizeof(activation)) {
		err_verbose_exit("Sync your number of activations and layers !");
	}
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	// malloc model
	nn_Model* model = createModel(num_layers, model_architecture, activations, loss, optimizer);
	verbose("Model created");
	model->printModelArchitecture(model);

	// load the dataset
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/numeric-", &shape, true);

	nn_Session* session = createSession(
		dataset,
		5,
		0.1,
		false,
		true,
		0.05,
		"avg-loss.log",
		"avg-right.log"
	);

	verbose("Session created");

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	setVerbose(true);
	verbose("Saving the model...");
	model->saveModel(model, "save/numeric-");

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
