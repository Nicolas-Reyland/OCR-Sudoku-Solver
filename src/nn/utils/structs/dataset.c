#include "dataset.h"
#include <stdlib.h>

nn_DataSet* nn_createDataSet(nn_Data* train_data, nn_Data* test_data)
{
	nn_DataSet* set = mem_malloc(sizeof(nn_DataSet));
	set->train 		= train_data;
	set->test 		= test_data;

	return set;
}

void _nn_freeDataSet(nn_DataSet* data_set)
{

	if(data_set == NULL)
    {
        nn_err_nn_verbose("freeDataSet: dataSet is null.");
		return;
    }
	_nn_freeData(data_set->test);
	_nn_freeData(data_set->train);
	mem_free(data_set);
}
