// test.c

#include "nn/nn.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"

extern linked_list* GPL;
extern bool _nn_random_init_done;

void printLayer(int, nn_Layer* layer);

int main()
{
	setVerbose(false);

	// init random
	// initRandom(); // NOT initializing random this ways
	// custom random init
	srand( 1234567890 );
	_nn_random_init_done = true;
	printf("Next random integer is: %d\n", rand());
	printf("Next random normalized double: %lf\n", getNormalizedRandomDouble());
	initMemoryTracking();
	// model architecture
	nn_ShapeDescription model_architecture[3] = {
		{ .dims = 1, .x = 2, .y = 1, .z = 1 },
		{ .dims = 1, .x = 3, .y = 1, .z = 1 },
		{ .dims = 1, .x = 1, .y = 1, .z = 1 }
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

	// here
	printLayer(0, &model->layers->input_layer);
	printLayer(1, &model->layers->hidden_layers[0]);
	printLayer(2, &model->layers->output_layer);

	// free model
	freeModel(model);
	free(GPL);

	return 0;
}

void printLayer(int index, nn_Layer* layer)
{
	printf("index: %d\n", index);
	nn_Node* node;
	for (size_t i = 0; i < layer->nb_nodes; i++) {
		node = layer->nodes[i];
		for (size_t j = 0; j < node->num_weights; j++) {
			printf("layers[%d]->nodes[%ld]->weights[%ld] = %lf\n", index, i, j, node->weights[j]);
		}
		printf("bias: %lf\n", node->bias);
	}
}
