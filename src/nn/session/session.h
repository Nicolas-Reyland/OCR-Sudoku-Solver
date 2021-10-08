// session.h

#ifndef NN_SESSION_H
#define NN_SESSION_H

#include "../data/data.h"
#include <stdbool.h>

typedef struct nn_Session {
	nn_Data* data;
	uint nb_epochs;
	double loss_threshold;
	bool stop_on_loss_threshold_reached;
	bool verbose;
	void (*train)(struct nn_Session* session, nn_Model* model);
	void (*test)(struct nn_Session* session,nn_Model* model);
} nn_Session;

#endif
