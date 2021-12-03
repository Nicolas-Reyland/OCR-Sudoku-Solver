// test-load-predict.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

int main()
{
	setVerbose(true);

	// init random
	nn_initRandom();
	nn_initMemoryTracking();

	verbose("Loading model...");
	// malloc model
	nn_Model* model = nn_loadModel("save/mnist/5k-");
    model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadTestOnlyDataSet("datas/mnist/", &shape, true);

	nn_Session* session = nn_createSession(
		dataset,
		0,
		0.1,
		false,
		true,
		0.1,
		NULL,
		NULL
	);

	verbose("Session created");

	session->test_one_hot(session, model);

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
