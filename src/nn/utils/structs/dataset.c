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
        verbose("freeDataSet: dataSet is null.");
        exit(EXIT_FAILURE);
    }
	verbose("Freeing test data...");
	_nn_freeData(data_set->test,true);
	verbose("Test data freed !");

	verbose("Freeing train data...");
	_nn_freeData(data_set->train,true);
	verbose("Train data freed !");

	verbose("Freeing dataset...");
	mem_free(data_set);
	verbose("Dataset freed !");
}
