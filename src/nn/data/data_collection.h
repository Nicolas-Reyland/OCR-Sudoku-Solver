// data_collection.h

#ifndef NN_DATA_COLLECTION_H
#define NN_DATA_COLLECTION_H

#include "in_out_tuple.h"
#include "utils/mem/mem-management.h"
#include <stdlib.h>

typedef struct nn_DataCollection // needs to be defined as a pointer please
{
    size_t num_tuples;
    nn_InOutTuple *iot_array;
    double *in_ptr;
    double *out_ptr;
} nn_DataCollection;

nn_DataCollection _nn_loadDataCollection(nn_InOutTuple *iot_array,
                                         size_t num_tuples, double *in_ptr,
                                         double *out_ptr);
void _nn_freeDataCollection(nn_DataCollection collection);

#endif
