// test-load-predict.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

int main(int argc, char** argv)
{
	setVerbose(true);

	if (argc != 2)
		err_verbose_exit("Usage: ./test-real-output model-name");
	char* model_path = argv[1];

	// init random
	initRandom();
	initMemoryTracking();

	verbose("Loading model...");
	// malloc model
	nn_Model* model = nn_loadModel(model_path);
    model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_ShapeDescription shape = emptyShapeDescription();
	// nn_Data* data = nn_loadSingleDataInputOutput("datas/numeric-data.in", "datas/numeric-data.out", &shape, true, "Loading numeric data");
    // nn_DataTuple data_tuple = data->splitTrainTest(data, 0.3);
    // nn_DataSet* dataset = nn_createDataSet(data_tuple.data1, data_tuple.data2);
	nn_DataSet* dataset = nn_loadTestOnlyDataSet("new-trainnn-", &shape, true);

	nn_Session* session = createTestSession(
		dataset, true
	);

	session->test_one_hot(session, model);

	// free model
	freeModel(model);
	// free session
	freeSession(session);

	free(GPL);

	return 0;
}