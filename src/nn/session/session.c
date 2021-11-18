// session.c

#include "session.h"

void _nn_train(struct nn_Session* session, nn_Model* model)
{
	// TODO: make this an argument, in session or directly here
	size_t nb_verb_step = 100;

	nn_InOutTuple** tuple_array = iot_linked_list_to_array(
		session->dataset->train->data_collection->data);
	size_t sample_size =session->dataset->train->data_collection->data->length;

	bool loss_threshold_condition = true;

	// incrementing directly epoch : so the verbose does not have to add 1
	// when printing (first epoch : 1, not 0)
	for (size_t epoch = 0; epoch++ < session->nb_epochs && loss_threshold_condition;)
	{
		if(session->verb_mode)
		{
			verbose("Epoch number: %ld", epoch);
		}
		shuffleArray(tuple_array, sample_size);
		size_t i = 0;
		double loss_buffer = 0;
		while(i < sample_size)
		{
			_nn_feedForward(model,tuple_array[i]->input->values);

			//we calculate the loss function
			double error = applyLosses(
				model->layers[model->num_layers - 1],
				tuple_array[i]->output->values,
				model->loss);

			loss_buffer += error;

			_nn_backPropagation(model, tuple_array[i]->output->values);
			_nn_updateWeights(model, session->learning_rate);
			i++;

			if (session->verb_mode && i % nb_verb_step == 0) {
				verbose("Losses error = %lf", error);
			}
		}

		loss_buffer /= sample_size;
		// we continue as long as we do not reached loss threshold
		// or we continue as long as we have epochs to do
		loss_threshold_condition =
			!session->stop_on_loss_threshold_reached ||
			loss_buffer > session->loss_threshold;
	}
	return;
}

void _nn_test(struct nn_Session* session, nn_Model* model)
{
	nn_InOutTuple** tuple_array = iot_linked_list_to_array(
		session->dataset->test->data_collection->data);
	size_t sample_size =session->dataset->test->data_collection->data->length;

	shuffleArray(tuple_array, sample_size);
	for(size_t i = 0; i < sample_size; i++)
	{
		verbose("Testing Tuple:");
		tuple_array[i]->printTuple(tuple_array[i]);
		_nn_feedForward(model, tuple_array[i]->input->values);

		verbose("Result:");
		for(size_t j = 0; j < model->layers[model->num_layers - 1]->num_nodes; j++)
			verbose_no_endline("%lf ",
			model->layers[model->num_layers - 1]->nodes[j]->value);
		verbose_endline();
		//we calculate the loss function
		double error = applyLosses(
			model->layers[model->num_layers - 1],
			tuple_array[i]->output->values,
			model->loss);
		verbose("Losses error = %f",error);
	}
}

void _nn_test_one_hot(struct nn_Session* session, nn_Model* model)
{
	// TODO: add an average right prediction for each value of the one-hot values
	verbose("Testing the model as one-hot");
	size_t num_steps_verb = 100;

	nn_InOutTuple** tuple_array = iot_linked_list_to_array(
		session->dataset->test->data_collection->data);
	size_t num_samples = session->dataset->test->data_collection->data->length;
	double loss_sum = 0;
	int num_right_predictions = 0;
	nn_Layer* output_layer = model->layers[model->num_layers - 1];

	for(size_t i = 0; i < num_samples; i++)
	{
		// verbosity
		if (session->verb_mode) {
			if (num_steps_verb % 100 == 0)
				verbose(" test session run: %.4g\% (%lu/%lu) done",
					100.0 * (double)num_steps_verb/num_samples,
					num_steps_verb,
					num_samples);
			num_steps_verb++;
		}
		// model prediction
		_nn_feedForward(model, tuple_array[i]->input->values);
		// get maximised value by the model
		size_t max_index = 0;
		for(size_t j = 1; j < output_layer->num_nodes; j++)
			if (output_layer->nodes[j]->value > output_layer->nodes[max_index]->value)
				max_index = j;
		// get index of hot value (1.0) in output values
		size_t result_index = 0;
		for (; result_index < tuple_array[i]->output->num_values; result_index++)
			if (tuple_array[i]->output->values[result_index]) // != 0
				break;
		// indices must be the same for the model to have rightly predicted
		if (max_index == result_index)
			num_right_predictions++;
		// calculate the loss
		double error = applyLosses(
			model->layers[model->num_layers - 1],
			tuple_array[i]->output->values,
			model->loss);
		// add the loss to a total (for later average loss calculation)
		loss_sum += error;
	}
	// print averages
	double avg_loss = (double)loss_sum / (double)num_samples;
	double avg_right_predictions = (double)num_right_predictions / (double)num_samples;
	// print averages
	verbose(" test session run: finished");
	verbose("loss avg: %lf", avg_loss);
	verbose("Right predictions avg: %lf%", 100.0 * avg_right_predictions);
}

nn_Session* createSession(nn_DataSet* dataset, unsigned int nb_epochs,
double loss_threshold, bool stop_on_loss_threshold_reached, bool verbose,
double learning_rate)
{
	nn_Session* session = mem_malloc(sizeof(nn_Session));

	session->dataset 			= dataset;
	session->nb_epochs 			= nb_epochs;
	session->loss_threshold 	= loss_threshold;
	session->verb_mode			= verbose;
	session->learning_rate		= learning_rate;

	session->stop_on_loss_threshold_reached = stop_on_loss_threshold_reached;

	session->train			= &_nn_train;
	session->test		 	= &_nn_test;
	session->test_one_hot	= &_nn_test_one_hot;

	return session;
}

void freeSession(nn_Session* session)
{
	_nn_freeDataSet(session->dataset);
	mem_free(session);
}
