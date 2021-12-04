// test-load-predict.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

int main(int argc, char** argv)
{
	setVerbose(true);

	if (argc != 2)
		err_verbose_exit("Usage: ./test-model-cli model-name");
	char* model_path = argv[1];

	// init random
	nn_initRandom();
	initMemoryTracking();

	verbose("Loading model...");
	// malloc model
	nn_Model* model = nn_loadModel(model_path);
    model->printModelArchitecture(model);

	// load the dataset
	verbose("Loading dataset...");
	nn_ShapeDescription shape = nn_emptyShapeDescription();
	// nn_Data* data = nn_loadSingleDataInputOutput("datas/numeric-data.in", "datas/numeric-data.out", &shape, true, "Loading numeric data");
    // nn_DataTuple data_tuple = data->splitTrainTest(data, 0.3);
    // nn_DataSet* dataset = nn_createDataSet(data_tuple.data1, data_tuple.data2);
	nn_DataSet* dataset = nn_loadTestOnlyDataSet("datas/numeric-1k-", &shape, true);

	nn_Session* session = nn_createTestSession(
		dataset, true
	);

	session->test_one_hot(session, model);

	setVerbose(true);

	// free model
	nn_freeModel(model);
	// free session
	nn_freeSession(session);

	mem_freeGPL(false);

	return 0;
}