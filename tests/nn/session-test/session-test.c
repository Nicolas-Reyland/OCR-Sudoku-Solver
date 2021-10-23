	// session-test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;
extern bool _nn_random_init_done;

void oneLearningStep(nn_Model* model, double* input, double* output, double learning_rate)
{
	_nn_feedForward(model, input);
	verbose("\tResult 1 : %lf", model->layers[model->num_layers - 1]->nodes[0]->value);
	_nn_backPropagation(model, output);
	verbose("\tUpdating weights");
	_nn_updateWeights(model, learning_rate);
	_nn_feedForward(model, input);
	verbose("\tResult 2 : %lf", model->layers[model->num_layers - 1]->nodes[0]->value);
}

int main(int argc, char** argv)
{
	setVerbose(true);
	srand( 1234567890 );
	_nn_random_init_done = true;
	printf("Next random integer is: %d\n", rand());
	initMemoryTracking();

	// get path to project as arg
	char input_path[255], output_path[255];
	if (argc != 2) {
		fprintf(stderr, "Must give path to root of project as first argument\n");
		exit(EXIT_FAILURE);
	}
	char* path_to_project = argv[1];
	strcpy(input_path, path_to_project);
	strcpy(output_path, path_to_project);
	strcat(input_path, "/datas/xor/input.txt");
	strcat(output_path, "/datas/xor/output.txt");

	nn_ShapeDescription train_description;
	nn_ShapeDescription test_description;

	nn_Data* train;
	nn_Data* test;

	train = nn_loadSingleDataInputOutput(
		input_path,
		output_path,
		&train_description,
		false
	);
	verbose("Created train data");
	test = nn_loadSingleDataInputOutput(
		input_path,
		output_path,
		&test_description,
		false
	);
	verbose("Created test data");

	nn_DataSet* dataset = nn_createDataSet(train,test);
	verbose("Created dataset.");

	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		train_description,
		create1DShapeDescription(2),
		create1DShapeDescription(1),
	};
	// activation functions
	activation activations[2] = {
		SIGMOID, RELU
	};
	// loss & optimizers
	losses loss = MEANSQUAREDERROR;
	optimizer optimizer = ADAM;
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);

	double weights[3][2] = {
    { 0.840188, 0.394383 },
    { 0.783099, 0.798440 },
    { 0.911647, 0.335223 },
  };

  double bias[3] = {
    0.197551,
    0.768230,
    0.277775,
  };

  // Initialize the weights
  model->layers[0]->nodes[0]->weights[0] = weights[0][0];
  model->layers[0]->nodes[0]->weights[1] = weights[0][1];
  model->layers[0]->nodes[1]->weights[0] = weights[1][0];
  model->layers[0]->nodes[1]->weights[1] = weights[1][1];
  model->layers[1]->nodes[0]->weights[0] = weights[2][0];
  model->layers[1]->nodes[1]->weights[0] = weights[2][1];

  // Initialize the bias
  model->layers[1]->nodes[0]->bias = bias[0];
  model->layers[1]->nodes[1]->bias = bias[1];
  model->layers[2]->nodes[0]->bias = bias[2];

	nn_Session* session = createSession(dataset, 20000, 0.0000001, false, false, 0.15);
	setVerbose(false);
	session->train(session, model);
	session->test(session, model);

	// free model
	freeModel(model);
	//free session (and dataset)
	//freeSession(session);
	free(GPL);

	return 0;
}
