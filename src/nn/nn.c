// nn.c

#include "nn.h"

bool _nn_global_init_done = false;

void nn_init(void) {
    initMemoryTracking();
    nn_initRandom();
    _nn_global_init_done = true;
}

//
