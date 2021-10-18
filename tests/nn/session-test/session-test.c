// session-test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;
extern bool _nn_random_init_done;

void printModelLayers(nn_ModelLayers* model_layer);
void printModelLayersValues(nn_ModelLayers* model_layer);


int main()
{
	setVerbose(true);
	// init random
	// initRandom(); // NOT initializing random this ways
	// custom random init
	srand( 1234567890 );
	_nn_random_init_done = true;
	printf("Next random integer is: %d\n", rand());
	initMemoryTracking();

	nn_ShapeDescription train_description;
	nn_ShapeDescription test_description;

	nn_Data* train;
	nn_Data* test;

	train = nn_DataLoadRaw(
		"/home/lilian/root/input.txt",
		"/home/lilian/root/output.txt",
		&train_description);
	verbose("Created train data");
	test = nn_DataLoadRaw(
		"/home/lilian/root/copy/input.txt",
		"/home/lilian/root/copy/output.txt",
		&test_description);
	verbose("Created test data");


	nn_DataSet* dataset = _nn_createDataSet(train,test);	
	verbose("Created dataset.");

	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		train_description,
		{ .dims = 1, .x = 2, .y = 1, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 }
	};
	// activation functions
	activation activations[3] = {
		RELU,RELU, SIGMOID
	};
	// loss & optimizers
	losses loss = MEANSQUAREDERROR;
	optimizer optimizer = ADAM;
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);

	nn_Session* session = createSession(dataset,1000,0.01,true,false,0.0001);
	session->train(session,model);
	session->test(session,model);

	// free model
	freeModel(model);
	//free session (and dataset)
	freeSession(session);
	free(GPL);

	return 0;
}

