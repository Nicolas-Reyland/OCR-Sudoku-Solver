#include "nn.h"

int main(void) {

	/* ----- Example program for creating, training and saving a nn-model ----- */

	/* --- Creating the model --- */
	nn_Model model = {
		.name = "Example Model",
		.num_layers = 3,
		.optiingizer = optimizers.RMSPROP,
		.loss = losses.CategoricalCrossentropy
	};

	/* --- Declaring the first Layer --- */
	nn_Utils_Shape shape_layer_1 = (nn_Utils_Shape) { .dims = 2, .X = 16, .Y = 16 };

	nn_Layers_Dense layer_1 = {
		.type = "Dense",
		.shape = shape_layer_1,
		.activation = activations.Relu,
	};

	model.add_dense_layer(layer_1);

	/* --- Declaring the second Layer --- */
	nn_Utils_Shape shape_layer_2 = (nn_Utils_Shape) { .dims = 1, .X = 64 };

	nn_Layers_Dense layer_2 = {
		.type = "Dense",
		.shape = shape_layer_2,
		.activation = activations.Relu,
	};

	model.add_dense_layer(layer_2);

	/* --- Declaring the third (last) Layer --- */
	nn_Utils_Shape shape_layer_3 = (nn_Utils_Shape) { .dims = 1, .X = 10};

	nn_Layers_Dense layer_3 = {
		.type = "Dense",
		.shape = shape_layer_3,
		.activation = activations.Relu,
	};

	model.add_dense_layer(layer_3);

	/* --- Loading the data --- */
	nn_Data data = nn_data_load_files("data/train_X", "data/train_Y", "data/test_X", "data/test_Y");
	/* OR :
	 * nn_RawData raw_data = nn_data_load_raw("data/raw_data");
	 * nn_Data data = nn_data_split_train_test(*raw_data, 0.70);
	 */


	/* --- Setting up the training --- */
	nn_Utils_TrainData train_info = {
		.data = *data;
		.num_epochs = 10,
		.batch_size = 16,
		.batches_per_epoch = 8,
		.loss_threshold = 1.4e-5,
		.stop_on_loss_threshold_reached = true,
		.verbose = true
	};

	/* --- Training & Testing the model --- */
	model.train(train_info);
	model.test();

	/* --- Saving the model and its weights/bias --- */
	model.save_architecture("models/{NAME}/architecture.json");
	model.save_weights_and_bias("models/{NAME}/weights_and_bias.h5");

	return 0;
}
