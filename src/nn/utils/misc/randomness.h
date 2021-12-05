// randomness.h

#ifndef NN_RANDOMNESS_H
#define NN_RANDOMNESS_H

#include "nn/data/in_out_tuple.h"
#include "utils/verbosity/verbose.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static const double D_RAND_MAX = RAND_MAX;
static const double RAND_MAX_DIV_BY_2 = RAND_MAX / 2;

void nn_initRandom();
void _nn_assertRandomInitialization();
double getRandomDouble(double min_value, double max_value);
double getNormalizedPositiveRandomDouble();
double getNormalizedRandomDouble();

void shuffleArray(nn_InOutTuple *array, size_t n);

#endif
