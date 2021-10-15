// data_collection.c

#include "data_collection.h"

nn_DataCollection* _nn_loadDataCollection(iot_linked_list* data)
{
    nn_DataCollection* data_collection = mem_malloc(sizeof(nn_DataCollection));
    data_collection->data = data;

    return data_collection;
}


void _nn_freeDataCollection(nn_DataCollection* collection)
{
    free_iot_linked_list(collection->data);
    mem_free(collection);
}