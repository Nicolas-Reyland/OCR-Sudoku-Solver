// data_collection.c

#include "data_collection.h"

nn_DataCollection* _nn_loadDataCollection(nn_InOutTuple** iot_array, size_t num_tuples)
{
    nn_DataCollection* data_collection = mem_malloc(sizeof(nn_DataCollection));
    data_collection->iot_array = iot_array;
    data_collection->num_tuples = num_tuples;

    return data_collection;
}


void _nn_freeDataCollection(nn_DataCollection* collection)
{
    if(collection == NULL)
    {
        nn_err_nn_verbose_exit("freeDataCollection: collection is null.\n");
    }
    for (size_t i = 0; i < collection->num_tuples; i++) {
        _nn_freeInOutTuple(collection->iot_array[i]);
    }
    mem_free(collection);
}
