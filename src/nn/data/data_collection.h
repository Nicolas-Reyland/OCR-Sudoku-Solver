// data_collection.h

#ifndef NN_DATA_COLLECTION_H
#define NN_DATA_COLLECTION_H

#include "in_out_tuple_linked_list.h"
#include "mem/mem-management.h"
#include <stdlib.h>

typedef struct nn_DataCollection //needs to be defined as a pointer please
{
    iot_linked_list* data;           //will contain a list of input/output tuple list
} nn_DataCollection;

nn_DataCollection* loadDataCollection(iot_linked_list* data);
void freeDataCollection(nn_DataCollection* collection);

#endif
