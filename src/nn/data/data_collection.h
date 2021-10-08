// data_collection.h

#ifndef NN_DATA_COLLECTION_H
#define NN_DATA_COLLECTION_H

// code comes here


#include "../utils/structs/linked_list.h"
#include "data_collection_tuple.h"

typedef struct nn_DataCollection //needs to be defined as a pointer please
{
    LinkedList* data;           //will contain a list of input/output tuple list
} nn_DataCollection;

nn_DataCollection* loadDataCollection(LinkedList* data);
void freeDataCollection(nn_DataCollection collection);

#endif
