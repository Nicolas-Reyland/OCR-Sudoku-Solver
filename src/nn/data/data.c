// data.c

#include "data.h"

/// <Summary>
/// Splits a data struct into two data struct, based on a splitting percentage
/// Then frees the current data, in order to not have memory leaks
/// <Summary/>
nn_DataTuple _nn_dataSplitTrainTest(nn_Data* data, int splittingPercentage)
{
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

    // add data to tuple struct at creation (and stop gcc from complaining)
    nn_DataTuple data_tuple = {
        .data1 = _nn_createData(_nn_loadDataCollection(data1)),
        .data2 = _nn_createData(_nn_loadDataCollection(data2))
    };

    _nn_freeData(data,false);
    return data_tuple;
}

static void _nn_printData(nn_Data* data)
{
    iot_ll_node* node = data->data_collection->data
        ->head;
    for(size_t i = 0; i < (size_t)data->data_collection->data->length;i++)
    {
        nn_InOutTuple* tuple = node->value;
        if(tuple == NULL)
        {
            fprintf(stderr, "tuple is not defined, exiting...");
            exit(EXIT_FAILURE);
        }
        printf("%ld/\n",(i+1));
        tuple->printTuple(tuple);

        node = node->next;
    }
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
    data->printData = &_nn_printData;

    return data;
}

/// <Summary>
/// Delete the data from memory
/// <Summary/>
void _nn_freeData(nn_Data* data,bool free_value)
{
    if(data == NULL)
    {
        verbose("freeData: data is null.");
        exit(EXIT_FAILURE);
    }
    _nn_freeDataCollection(data->data_collection,free_value);
    mem_free(data);
}
