// test-train-save.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main()
{
	nn_setVerbose(true);

	// init random
	nn_initRandom();
	nn_initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[] = {
		nn_create2DShapeDescription(28, 28),
		nn_create1DShapeDescription(200),
		nn_create1DShapeDescription(9),
	};
	const size_t num_layers = sizeof(model_architecture) / sizeof(nn_ShapeDescription);
	const size_t num_activations = num_layers - 1;
	// activation functions
	activation activations[] = {
		SIGMOID, SOFTMAX
	};
	if (sizeof(activations) != num_activations * sizeof(activation)) {
		nn_err_nn_verbose_exit("Sync your number of activations and layers !");
	}
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	// malloc model
	nn_Model* model = nn_createModel(num_layers, model_architecture, activations, loss, optimizer);
	nn_verbose("Model created");
	model->printModelArchitecture(model);

	// load the dataset
	nn_ShapeDescription shape = nn_emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/numeric-", &shape, true);

	nn_Session* session = nn_createSession(
		dataset,
		5,
		0.1,
		false,
		true,
		0.05,
		"logs/avg-loss.log",
		"logs/avg-right.log"
	);

	nn_verbose("Session created");

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	nn_setVerbose(true);
	nn_verbose("Saving the model...");
	model->saveModel(model, "save/numeric-");

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	nn_verbose("Model freed.");

	mem_freeGPL(false);

	return 0;
}
