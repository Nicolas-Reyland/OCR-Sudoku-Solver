// session.h

#ifndef NN_SESSION_H
#define NN_SESSION_H

#define VERB_UPDATE_STEP 100

#include "nn/model/model.h"
#include "nn/utils/misc/randomness.h"
#include "nn/utils/session/evaluate.h"
#include "nn/utils/structs/dataset.h"
#include "utils/misc/logging.h"
#include "utils/verbosity/progressbar.h"
#include "utils/verbosity/verbose.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct nn_Session nn_Session;
struct nn_Session {
    nn_DataSet *dataset;
    unsigned int nb_epochs;
    double loss_threshold;
    bool stop_on_loss_threshold_reached;
    bool verb_mode;
    double learning_rate;
    const char *loss_log_file;
    const char *right_log_file;
    void (*train)(struct nn_Session *session, nn_Model *model);
    void (*train_one_hot)(struct nn_Session *session, nn_Model *model);
    void (*test)(struct nn_Session *session, nn_Model *model);
    void (*test_one_hot)(struct nn_Session *session, nn_Model *model);
};

nn_Session *nn_createSession(nn_DataSet *dataset, unsigned int nb_epochs,
                             double loss_threshold,
                             bool stop_on_loss_threshold_reached, bool verbose,
                             double learning_rate, const char *loss_log_file,
                             const char *right_log_file);
nn_Session *nn_createTestSession(nn_DataSet *dataset, bool verbose);
void nn_freeSession(nn_Session *session);

#endif
