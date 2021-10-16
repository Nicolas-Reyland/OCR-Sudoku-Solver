// randomness.h

#ifndef NN_RANDOMNESS_H
#define NN_RANDOMNESS_H

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include "nn/utils/misc/normal_distribution.h"

static const double D_RAND_MAX = RAND_MAX;
static const double RAND_MAX_DIV_BY_2 = RAND_MAX / 2;

void initRandom();
void _nn_assertRandomInitialization();
double getRandomDouble(double min_value, double max_value);
double getNormalizedPositiveRandomDouble();
double getNormalizedRandomDouble();

#endif
