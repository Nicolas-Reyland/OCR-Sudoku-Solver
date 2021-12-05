// test-load-predict.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

int main()
{
	setVerbose(true);

	// init random
	nn_initRandom();
	initMemoryTracking();

	verbose("Loading model...");
	// malloc model
	nn_Model* model = nn_loadModel("save/emnist-big-");
        model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_DataSet dataset = nn_loadTestOnlyDataSet("datas/emnist-", true);

	nn_Session* session = nn_createTestSession(
		&dataset,
		true
	);

	session->test_one_hot(session, model);

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	mem_freeGPL(false);

	return 0;
}
