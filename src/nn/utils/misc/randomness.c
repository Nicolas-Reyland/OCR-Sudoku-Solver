// randomness.c

#include "randomness.h"

bool _nn_random_init_done = false;

/* Initialize randomness
 */
void initRandom()
{
  if (_nn_random_init_done)
    return;
  srand( time( NULL ));
  _nn_random_init_done = true;
}

/* Assert random initialization
 */
void _nn_assertRandomInitialization(void)
{
  if (!_nn_random_init_done) {
    fprintf(stderr, "Randomness has not been initalized. Exiting (this is an assertion).\n");
    exit(EXIT_FAILURE);
  }
}

/* Returns a random double value between min_value and max_value
 */
double getRandomDouble(double min_value, double max_value)
{
  double range = max_value - min_value;
  double random_double = (double) rand() / (D_RAND_MAX / range);
  return random_double + min_value;
}

/* Returns a random double value between -1.0 and 1.0
 */
double getNormalizedRandomDouble()
{
  return ltqnorm(rand() / D_RAND_MAX);
}

//
