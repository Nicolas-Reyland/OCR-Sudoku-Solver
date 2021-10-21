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
	// init random
	// initRandom(); // NOT initializing random this ways
	// custom random init
	srand( 1234567890 );
	_nn_random_init_done = true;
	printf("Next random integer is: %d\n", rand());
	initMemoryTracking();
	
	verbose("init done");

	// get path to project as arg
	char input_path[255], output_path[255];
	char* path_to_project = argv[1];
	strcpy(input_path, path_to_project);
	strcpy(output_path, path_to_project);
	strcat(input_path, "/datas/xor/input.txt");
	strcat(output_path, "/datas/xor/output.txt");

	nn_ShapeDescription train_description;
	nn_ShapeDescription test_description;

	nn_Data* train;
	nn_Data* test;

	train = nn_DataLoadRaw(
		input_path,
		output_path,
		&train_description);
	verbose("Created train data");
	test = nn_DataLoadRaw(
		input_path,
		output_path,
		&test_description);
	verbose("Created test data");

	nn_DataSet* dataset = _nn_createDataSet(train,test);	
	verbose("Created dataset.");

	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		train_description,
		{ .dims = 1, .x = 2, .y = 1, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 },
	};
	// activation functions
	activation activations[2] = {
		SIGMOID, SIGMOID
	};
	// loss & optimizers
	losses loss = MEANSQUAREDERROR;
	optimizer optimizer = ADAM;
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);

	/*nn_Session* session = createSession(dataset,1000,0.01,true,false,0.0001);
	session->train(session,model);
	session->test(session,model);*/

	// one manual forward/backward propagation
	double input_collection[4][2] = {
		{ 1.0, 1.0 },
		{ 1.0, 0.0 },
		{ 0.0, 1.0 },
		{ 0.0, 0.0 },
	};
	double output_collection[4][1] = {
		{ 0.0 },
		{ 1.0 },
		{ 1.0 },
		{ 0.0 },
	};
	
	/*setVerbose(false);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			double* input = input_collection[j];
			double* output = output_collection[j];
			verbose("---------------------------");
			oneLearningStep(model, input, output, 0.3);
			verbose("\tSupposed: %lf", output[0]);
		}
	}*/
	setVerbose(true);
	for (int j = 0; j < 4; j++) {
		double* input = input_collection[j];
		double* output = output_collection[j];
		verbose("---------------------------");
		oneLearningStep(model, input, output, 0.2);
		verbose("\tSupposed: %lf", output[0]);
	}

	// free model
	freeModel(model);
	//free session (and dataset)
	//freeSession(session);
	free(GPL);

	return 0;
}

