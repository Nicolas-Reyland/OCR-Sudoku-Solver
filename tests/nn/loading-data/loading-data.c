// loading-data.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;
extern bool _nn_random_init_done;


int main()
{
	setVerbose(true);

	initMemoryTracking();
	/*
	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		{ .dims = 1, .x = 3, .y = 1, .z = 1 },
		{ .dims = 1, .x = 4, .y = 1, .z = 1 },
		{ .dims = 1, .x = 2, .y = 1, .z = 1 }
	};
	// activation functions
	activation activations[3] = {
		SIGMOID, SIGMOID, SOFTMAX
	};
	// loss & optimizers
	losses loss = CATEGORICALCROSSENTROPY;
	optimizer optimizer = ADAM;
	// malloc model
	nn_Model* model = createModel(3, model_architecture, activations, loss, optimizer);
	*/
	nn_ShapeDescription* description;

	nn_Data* data = nn_DataLoadRaw(
		"datas/xor/input.txt",
		"datas/xor/output.txt",
		description);
	printf("created data.\n");
	data->printData(data);

	//free data
	_nn_freeData(data,true);

	// free model
	//freeModel(model);
	free(GPL);

	return 0;
}
