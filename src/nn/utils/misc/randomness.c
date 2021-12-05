// randomness.c

#include "randomness.h"

bool _nn_random_init_done = false;

/* Initialize randomness
 */
void nn_initRandom() {
    if (_nn_random_init_done)
        return;
    srand(time(NULL));
    _nn_random_init_done = true;
}

/* Assert random initialization
 */
void _nn_assertRandomInitialization(void) {
    if (!_nn_random_init_done) {
        err_verbose_exit("Randomness has not been initalized. Exiting (this is "
                         "an assertion).\n");
    }
}

/* Returns a random double value between min_value and max_value
 */
double getRandomDouble(double min_value, double max_value) {
    double range = max_value - min_value;
    double random_double = (double)rand() / (D_RAND_MAX / range);
    return random_double + min_value;
}

/* Returns a random double value between -1.0 and 1.0
 */
double getNormalizedRandomDouble() {
    return ((double)rand() / RAND_MAX_DIV_BY_2) - 1.0;
}

//
void shuffleArray(nn_InOutTuple *array, size_t n) {
    if (n > 1) {
        size_t i;
        for (i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            nn_InOutTuple t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}