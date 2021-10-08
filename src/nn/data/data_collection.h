// data_collection.h

#ifndef NN_DATA_COLLECTION_H
#define NN_DATA_COLLECTION_H

// code comes here


#include "../utils/structs/linked_list.h"


typedef struct nn_DataCollection //needs to be defined as a pointer please
{
    linked_list* data;           //will contain a list of input/output tuple list
} nn_DataCollection;

nn_DataCollection* loadDataCollection(linked_list* data);
void freeDataCollection(nn_DataCollection* collection);

#endif
