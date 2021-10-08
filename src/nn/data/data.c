#include "data.h"
#include <stdlib.h>

/// <Summary>
/// Splits a data struct into two data struct, based on a splitting percentage
/// Then frees the current data, in order to not have memory leaks
/// <Summary/>
nn_DataTuple _nn_dataSplitTrainTest(nn_Data* data, int splittingPercentage)
{
    nn_DataTuple data_tuple;

    LinkedList* list_to_split = data->data_collection->data;

    LinkedList* data1   = init_linked_list();
    LinkedList* data2   = init_linked_list();

    int total_size      = data->data_collection->data->;
    int split_nb        =  / splittingPercentage;

    for(int i = 0; i < split_nb; i++)
    {
        data1->append_value(data1,list_to_split->get_value_at(i)->element);
    }
    for (int i = split_nb; i < total_size; i++)
    {
        data2->insert(data2,list_to_split->get_value_at(i)->element);
    }


    data_tuple->data1->data_collection = loadDataCollection(data1);
    data_tuple->data2->data_collection = loadDataCollection(data2);

    freeData(data);

    return data_tuple;
}

/// <Summary>
/// Create a pointer to a data struct
/// <Summary/>
nn_Data* createData(nn_DataCollection* collection)
{
    //stuff comes here
    nn_Data* data = malloc(sizeof(nn_Data));
    data->data_collection = collection;
    data->splitTrainTest = &_nn_dataSplitTrainTest;

    return data;
}

/// <Summary>
/// Delete the data from memory
/// <Summary/>
void freeData(nn_Data* data)
{
  freeDataCollection(data->data_collection);
  free(data);
}
