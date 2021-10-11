// data.c

#include "data.h"

/// <Summary>
/// Splits a data struct into two data struct, based on a splitting percentage
/// Then frees the current data, in order to not have memory leaks
/// <Summary/>
nn_DataTuple _nn_dataSplitTrainTest(nn_Data* data, int splittingPercentage)
{
    nn_DataTuple data_tuple;

    iot_linked_list* list_to_split = data->data_collection->data;

    iot_linked_list* data1   = init_iot_linked_list();
    iot_linked_list* data2   = init_iot_linked_list();

    int total_size      = data->data_collection->data->length;
    int split_nb        =  total_size / splittingPercentage;

    for(int i = 0; i < split_nb; i++)
    {
        data1->append_value(data1,((iot_ll_node*)list_to_split->get_value_at(list_to_split,i))->value);
    }
    for (int i = split_nb; i < total_size; i++)
    {
        data2->append_value(data2,((iot_ll_node*)list_to_split->get_value_at(list_to_split,i))->value);
    }


    data_tuple.data1->data_collection = _nn_loadDataCollection(data1);
    data_tuple.data2->data_collection = _nn_loadDataCollection(data2);

    _nn_freeData(data);

    return data_tuple;
}

/// <Summary>
/// Create a pointer to a data struct
/// <Summary/>
nn_Data* _nn_createData(nn_DataCollection* collection)
{
    //stuff comes here
    nn_Data* data = mem_malloc(sizeof(nn_Data));
    data->data_collection = collection;
    data->splitTrainTest = &_nn_dataSplitTrainTest;

    return data;
}

/// <Summary>
/// Delete the data from memory
/// <Summary/>
void _nn_freeData(nn_Data* data)
{
  _nn_freeDataCollection(data->data_collection);
  mem_free(data);
}
