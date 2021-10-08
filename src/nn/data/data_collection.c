#include "data_collection.h"
#include <stdlib.h>


nn_DataCollection* loadDataCollection(LinkedList* data)
{
    nn_DataCollection data_collection = malloc(sizeof(nn_DataCollection));

    data_collection->data = data;

    return data_collection;
}


void freeDataCollection(nn_DataCollection collection)
{
    free_linked_list(collection->data);
    free(collection);
}
