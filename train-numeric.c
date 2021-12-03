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
		create2DShapeDescription(28, 28),
		create1DShapeDescription(200),
		create1DShapeDescription(9),
	};
	const size_t num_layers = sizeof(model_architecture) / sizeof(nn_ShapeDescription);
	const size_t num_activations = num_layers - 1;
	// activation functions
	activation activations[] = {
		TANH, SOFTMAX
	};
	if (sizeof(activations) != num_activations * sizeof(activation)) {
		err_verbose_exit("Sync your number of activations and layers !");
	}
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	// malloc model
	nn_Model* model = nn_createModel(num_layers, model_architecture, activations, loss, optimizer);
	verbose("Model created");
	model->printModelArchitecture(model);

	// load the dataset (~from scratch~)
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_Data* data = nn_loadSingleDataInputOutput("datas/numeric-data.in", "datas/numeric-data.out", &shape, true, "Loading numeric data");
    nn_DataTuple data_tuple = data->splitTrainTest(data, 0.3);
    nn_DataSet* dataset = nn_createDataSet(data_tuple.data1, data_tuple.data2);

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

	verbose("Session created");

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

    //model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	setVerbose(true);
	verbose("Saving the model...");
	model->saveModel(model, "save/numeric-tanh-sigmoid-200-");

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}