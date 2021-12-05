#include "tonumber.h"

int toNumber(double *input, size_t size) {
    // The function will send a number =/= 0 only if the
    // network is very sure about the result it is outputing

    size_t index = 0;
    size_t index2 = 0; // second-best index
    for (size_t i = 1; i < size; i++) {
        if (input[i] > input[index]) {
            index2 = index;
            index = i;
        } else if (input[i] > input[index2])
            index2 = i;
    }
    if (input[index] - input[index2] < DIFF_THRESHOLD) {
        if (index == 0) // special case for 1's
            return 1;
        return 0;
    }
    return (int)index + 1;
}
