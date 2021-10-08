// data_tuple.h

#ifndef NN_DATA_TUPLE_H
#define NN_DATA_TUPLE_H

#include "data.h"

typedef struct nn_DataTuple
{
    nn_Data* data1;
    nn_Data* data2;
} nn_DataTuple;

// here, no need of making a malloc to this struct, because we won't
// have to modify it, through a function, it will always be a const
#endif
