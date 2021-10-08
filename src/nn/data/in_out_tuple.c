#include "in_out_tuple.h"
#include <stdio.h>

nn_InOutTuple* createInOutTuple(nn_Sample* input, nn_Sample* output)
{
    nn_InOutTuple tuple = malloc(sizeof(nn_InOutTuple));

    tuple->input = input;
    tuple->output = output;

    return tuple;
}

void freeInOutTuple(nn_InOutTuple tuple)
{
    freeSample(tuple->input);
    freeSample(tuple->output);
    free(tuple);
}
