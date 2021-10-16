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
		shuffleArray(tuple_array,sample_size);
		size_t i = 0;
		while(i < sample_size && loss_threshold_condition)
		{
			_nn_feedForward(model->layers,tuple_array[i]->input->values);
			_nn_backPropagation(model,tuple_array[i]->output->values);
			
			//we calculate the loss function 
			double error = applyLosses(
			model->layers->output_layer,
			model->loss
			);
			// we continue as long as we do not reached loss threshold 
			// or we continue as long as we have epochs to do 
			loss_threshold_condition = 
				session->stop_on_loss_threshold_reached ||
				error > session->loss_threshold;
			i++;
		}
		epoch++;
	}
	return;
}

void _nn_test(struct nn_Session* session,nn_Model* model, nn_Data* test)
{
	//code goes here
	return;
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

