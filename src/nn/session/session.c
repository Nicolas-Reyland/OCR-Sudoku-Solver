// session.c

#include "session.h"

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
	nn_Session* session = mem_malloc(sizeof(nn_Session));

	session->data 			= data;
	session->nb_epochs 		= nb_epochs;
	session->loss_threshold 	= loss_threshold;
	session->verbose			= verbose;

	session->stop_on_loss_threshold_reached = stop_on_loss_threshold_reached;

	session->train 	= &_nn_train;
	session->test	= &_nn_test;

	return session;
}

void freeSession(nn_Session* session)
{
	_nn_freeData(session->data);
	mem_free(session);
}
