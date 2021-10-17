// session.c

#include "session.h"

void _nn_train(struct nn_Session* session, nn_Model* model)
{
	nn_InOutTuple** tuple_array = iot_linked_list_to_array(
		session->dataset->train->data_collection->data);
	size_t sample_size =session->dataset->train->data_collection->data->length;
	
	bool loss_threshold_condition = true;
	
	size_t epoch = 0;
	while (epoch < session->nb_epochs && loss_threshold_condition)
	{
		if(session->verbose)
		{
			verbose("Epoch number: %ld",epoch);
			verbose("Shuffling tuples...");
		}
		shuffleArray(tuple_array,sample_size);
		size_t i = 0;
		while(i < sample_size && loss_threshold_condition)
		{
			if(session->verbose)
			{
				verbose("Testing Tuple:");
				tuple_array[i]->printTuple(tuple_array[i]);			
			}
			_nn_feedForward(model->layers,tuple_array[i]->input->values);

			//we calculate the loss function 
			double error = applyLosses(
			&(model->layers->output_layer),
			tuple_array[i]->output->values,
			model->loss);
			if(session->verbose)
				verbose("Losses error = %f",error);

			if(session->verbose)
			{
				model->layers->printModelLayers(model->layers);
				model->layers->printModelLayersValues(model->layers);
			}
			// we continue as long as we do not reached loss threshold 
			// or we continue as long as we have epochs to do 
			loss_threshold_condition = 
				!session->stop_on_loss_threshold_reached ||
				error > session->loss_threshold;
			_nn_backPropagation(model,tuple_array[i]->output->values);
			_nn_updateWeights(model->layers,session->learning_rate);
			
			
			i++;
		}
		epoch++;
	}
	return;
}

void _nn_test(struct nn_Session* session,nn_Model* model)
{
	nn_InOutTuple** tuple_array = iot_linked_list_to_array(
		session->dataset->test->data_collection->data);
	size_t sample_size =session->dataset->test->data_collection->data->length;

	shuffleArray(tuple_array,sample_size);
	for(size_t i = 0; i < sample_size; i++)
	{
		verbose("Training Tuple:");
		tuple_array[i]->printTuple(tuple_array[i]);
		_nn_feedForward(model->layers,tuple_array[i]->input->values);
		
		verbose("Result:");
		for(size_t j = 0; j < model->layers->output_layer.nb_nodes; j++)
			verbose_no_endline("%f ",
			model->layers->output_layer.nodes[j]->value);
		verbose("");
		//we calculate the loss function 
		double error = applyLosses(
		&(model->layers->output_layer),
		tuple_array[i]->output->values,
		model->loss);
		verbose("Losses error = %f",error);
	}
}

nn_Session* createSession(nn_DataSet* dataset, unsigned int nb_epochs,
double loss_threshold, bool stop_on_loss_threshold_reached, bool verbose,
float learning_rate)
{
	nn_Session* session = mem_malloc(sizeof(nn_Session));

	session->dataset 			= dataset;
	session->nb_epochs 			= nb_epochs;
	session->loss_threshold 	= loss_threshold;
	session->verbose			= verbose;
	session->learning_rate		= learning_rate;

	session->stop_on_loss_threshold_reached = stop_on_loss_threshold_reached;

	session->train 	= &_nn_train;
	session->test	= &_nn_test;

	return session;
}

void freeSession(nn_Session* session)
{
	_nn_freeDataSet(session->dataset);
	mem_free(session);
}

