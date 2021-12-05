// nn.h

#ifndef NN_NN_H
#define NN_NN_H

#include "data/init_data.h"
#include "model/load_model.h"
#include "session/session.h"
#include "utils/mem/mem-management.h"
#include "utils/misc/randomness.h"
#include "utils/structs/dataset.h"

extern bool _nn_global_init_done;

void nn_init(void);

#endif
