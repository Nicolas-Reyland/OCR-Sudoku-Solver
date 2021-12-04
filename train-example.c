// train-example.c

#include "nn/nn.h"

// extern linked_list* GPL;

int main()
{
	// init things
	nn_initRandom();
	initMemoryTracking();
	setVerbose(true);

	// model architecture
	nn_ShapeDescription model_architecture[] = {
		nn_create2DShapeDescription(28, 28),
		nn_create2DShapeDescription(30, 30),
		nn_create1DShapeDescription(800),
		nn_create1DShapeDescription(200),
		nn_create1DShapeDescription(9),
	};
	// activation functions
	activation activations[] = {
		TANH, TANH, SIGMOID, SOFTMAX
	};
	// loss & optimizer
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	// malloc model
	nn_Model* model = nn_createModel(num_layers, model_architecture, activations, loss, optimizer);

	// load the dataset
    nn_DataSet dataset = nn_loadDataSet("datas/mnist/", true);

    // session
	nn_Session* session = nn_createSession(
		&dataset,
		10,
		0.1,
		false,
		true,
		0.05,
		"avg-loss.log",
		"avg-right.log",
	);

	session->train_one_hot(session, model);
	session->test_one_hot(session, model);

	// save model
	model->saveModel(model, save_model_str);

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);
	// free remaining pointers and the Global Pointer List itself
	mem_freeGPL(false);

	return 0;
}