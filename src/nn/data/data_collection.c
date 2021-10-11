#include "data_collection.h"
#include <stdlib.h>


nn_DataCollection* loadDataCollection(iot_linked_list* data)
{
    nn_DataCollection* data_collection = malloc(sizeof(nn_DataCollection));

    data_collection->data = data;

    return data_collection;
}


void freeDataCollection(nn_DataCollection* collection)
{
    free_iot_linked_list(collection->data);
    free(collection);
}
