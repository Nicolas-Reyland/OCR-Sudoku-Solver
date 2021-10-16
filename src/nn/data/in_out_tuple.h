// in_out_tuple.h

#ifndef IN_OUT_TUPLE_H
#define IN_OUT_TUPLE_H

#include "sample/sample.h"
#include <stdlib.h>

typedef struct nn_InOutTuple
{
    nn_Sample* input;
    nn_Sample* output;
    void (*printTuple)(nn_InOutTuple);
} nn_InOutTuple;

nn_InOutTuple* createInOutTuple(nn_Sample* input, nn_Sample* output);
void freeInOutTuple(nn_InOutTuple* tuple);

#endif
