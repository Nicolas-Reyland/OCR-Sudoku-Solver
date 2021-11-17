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
	nn_Model* model = nn_loadModel("save/xor/");

    model->printModelLayers(model);
    model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_ShapeDescription shape = emptyShapeDescription();
	nn_DataSet* dataset = nn_loadDataSet("datas/xor/", &shape, true);

	nn_Session* session = createSession(
		dataset,
		10,
		0.1,
		false,
		false,
		0.1
	);

	verbose("Session allocated");

	setVerbose(true);
	session->test(session, model);

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	verbose("Model freed.");

	free(GPL);

	return 0;
}
