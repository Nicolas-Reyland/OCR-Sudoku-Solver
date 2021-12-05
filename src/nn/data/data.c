// data.c

#include "data.h"

/// <Summary>
/// Splits a data struct into two data struct, based on a splitting percentage
/// Then frees the current data, in order to not have memory leaks
/// <Summary/>
nn_DataTuple _nn_dataSplitTrainTest(nn_Data data, double test_proportion) {
    nn_InOutTuple *array_to_split = data.data_collection.iot_array;

    size_t total_size = data.data_collection.num_tuples;
    size_t split_nb = (size_t)((double)total_size * (1.0 - test_proportion));
    size_t after_split_nb = total_size - split_nb;

    nn_InOutTuple *before_split = mem_malloc(split_nb * sizeof(nn_InOutTuple));
    nn_InOutTuple *after_split =
        mem_malloc(after_split_nb * sizeof(nn_InOutTuple));

    for (size_t i = 0; i < split_nb; i++) {
        before_split[i] = array_to_split[i];
    }
    for (size_t i = split_nb; i < total_size; i++) {
        after_split[i - split_nb] = array_to_split[i];
    }

    // add data to tuple struct at creation (and stop gcc from complaining)
    nn_DataTuple data_tuple = {.data1 = _nn_createData(_nn_loadDataCollection(
                                   before_split, split_nb, NULL, NULL)),
                               .data2 = _nn_createData(_nn_loadDataCollection(
                                   after_split, after_split_nb, NULL, NULL))};

    return data_tuple;
}

static void _nn_printData(nn_Data data) {
    for (size_t i = 0; i < data.data_collection.num_tuples; i++) {
        nn_InOutTuple tuple = data.data_collection.iot_array[i];
        printf("%ld/\n", (i + 1));
        tuple.printTuple(tuple);
    }
}

/// <Summary>
/// Create a data struct
/// <Summary/>
nn_Data _nn_createData(nn_DataCollection collection) {
    // stuff comes here
    nn_Data data;
    data.active = true;
    data.data_collection = collection;
    data.splitTrainTest = &_nn_dataSplitTrainTest;
    data.printData = &_nn_printData;

    return data;
}

/// <Summary>
/// Delete the data from memory
/// <Summary/>
void _nn_freeData(nn_Data data) {
    _nn_freeDataCollection(data.data_collection);
}
