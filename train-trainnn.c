// test-train-save.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/nn_verbose.h"

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
		nn_create1DShapeDescription(128),
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

	// load the dataset (~from scratch~)
	nn_ShapeDescription shape = nn_emptyShapeDescription();
	nn_Data* data = nn_loadSingleDataInputOutput("src/grid_detection/trainnn-data.in", "src/grid_detection/trainnn-data.out", &shape, true, "Loading numeric data");
//    nn_DataTuple data_tuple = data->splitTrainTest(data, 0.3);
    nn_DataSet* dataset = nn_createDataSet(data, data);

    // session
	nn_Session* session = nn_createSession(
		dataset,
		10,
		0.1,
		false,
		true,
		0.01,
		"avg-loss.log",
		"avg-right.log"
	);

	nn_verbose("Session created");

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	nn_setVerbose(true);
	nn_verbose("Saving the model...");
	model->saveModel(model, "save/trainn-128-");

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	nn_verbose("Model freed.");

	free(GPL);

	return 0;
}