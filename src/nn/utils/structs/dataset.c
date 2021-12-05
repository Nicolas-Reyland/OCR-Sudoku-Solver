#include "dataset.h"
#include <stdlib.h>

nn_DataSet nn_createDataSet(nn_Data train_data, nn_Data test_data) {
    nn_DataSet set;
    set.train = train_data;
    set.test = test_data;

    return set;
}

void _nn_freeDataSet(nn_DataSet data_set) {

    _nn_freeData(data_set.test);
    _nn_freeData(data_set.train);
}
