// test-load-predict.c

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

	verbose("Loading model...");
	// malloc model
	nn_Model* model = nn_loadModel("save/mnist/1k-");

    model->printModelLayers(model);
    model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/mnist/1k-", &shape, true);

	nn_Session* session = createSession(
		dataset,
		30,
		0.1,
		false,
		false,
		0.1
	);

	verbose("Session allocated");

	setVerbose(false);
	session->test_one_hot(session, model);

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
