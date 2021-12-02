// test-train-save.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;

int main(int argc, char** argv)
{
	setVerbose(true);
	if (argc != 4)
		err_verbose_exit("Usage: ./train-generic hidden-layer-size second-activation data-prefix");

	char* hidden_layer_size_str = argv[1];
	char* second_activation_str = argv[2];
	char* data_prefix = argv[3];

	size_t hidden_layer_size = (size_t)atoi(hidden_layer_size_str);
	activation second_activation;
	if (strcmp(second_activation_str, "sigmoid") == 0) {
		second_activation = SIGMOID;
	} else {
		if (strcmp(second_activation_str, "softmax") == 0)
			second_activation = SOFTMAX;
		else
			second_activation = SIGMOID;
	}
	verbose("Activation: %s", activation_str[second_activation]);
	char save_model_str[255];
	sprintf(save_model_str, "save/%s-%s-%s-", data_prefix, second_activation_str, hidden_layer_size_str);
	verbose("Will be saved as: %s", save_model_str);

	// init random
	initRandom();
	initMemoryTracking();

	// model architecture
	nn_ShapeDescription model_architecture[] = {
		create2DShapeDescription(28, 28),
		create1DShapeDescription(hidden_layer_size),
		create1DShapeDescription(9),
	};
	const size_t num_layers = sizeof(model_architecture) / sizeof(nn_ShapeDescription);
	const size_t num_activations = num_layers - 1;
	// activation functions
	activation activations[] = {
		SIGMOID, second_activation//SOFTMAX
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

	// load the dataset (~from scratch~)
	nn_ShapeDescription shape = emptyShapeDescription();
	char data_in_path[255];
	char data_out_path[255];
	strcpy(data_in_path, data_prefix);
	strcpy(data_out_path, data_prefix);
	strcat(data_in_path, "-data.in");
	strcat(data_out_path, "-data.out");
	nn_Data* data = nn_loadSingleDataInputOutput(data_in_path, data_out_path, &shape, true, "Loading data");
    nn_DataTuple data_tuple = data->splitTrainTest(data, 0.3);
    nn_DataSet* dataset = nn_createDataSet(data_tuple.data1, data_tuple.data2);

    // session
	nn_Session* session = createSession(
		dataset,
		10,
		0.1,
		false,
		true,
		0.1,
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
	model->saveModel(model, save_model_str);

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}