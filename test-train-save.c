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
	nn_ShapeDescription model_architecture[3] = {
		create1DShapeDescription(2),
		create1DShapeDescription(2),
		create1DShapeDescription(1),
	};
	// activation functions
	activation activations[2] = {
		RELU, SIGMOID
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;

	verbose("Allocating model...");
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);
	verbose("Model allocated");

	// load the dataset
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/xor/", &shape, true);
    model->printModelArchitecture(model);

	nn_Session* session = createSession(
		dataset,
		10,
		0.1,
		false,
		false,
		0.1
	);

	verbose("Session allocated");

	setVerbose(false);
	session->train(session, model);
	session->test(session, model);

    model->printModelLayers(model);
    model->printModelArchitecture(model);

	// save model
	setVerbose(false);
	verbose("Saving the model...");
	model->saveModel(model, "save/xor/");

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
