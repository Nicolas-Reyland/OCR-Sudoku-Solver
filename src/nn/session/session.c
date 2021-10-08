#include "session.h"
#include <stdlib.h>

void _nn_train(struct nn_Session* session, nn_Model* model)
{
	//code goes here
	return;
}

void _nn_test(struct nn_Session* session,nn_Model* model)
{
	//code goes here
	return;
}

nn_Session* createSession(nn_Data* data, unsigned int nb_epochs,
double loss_threshold, bool stop_on_loss_threshold_reached, bool verbose)
{
	nn_Session* session = malloc(sizeof(nn_Session));

	nn_Session->data 			= data;
	nn_Session->nb_epochs 		= nb_epochs;
	nn_Session->loss_threshold 	= loss_threshold;
	nn_Session->verbose			= verbose;

	nn_Session->stop_on_loss_threshold_reached = stop_on_loss_threshold_reached;

	nn_Session->train 	= &_nn_train;
	nn_Session->test	= &_nn_test;

	return nn_Session;
}

void freeSession(nn_Session* session)
{
	freeData(session->data);
	free(session);
}
