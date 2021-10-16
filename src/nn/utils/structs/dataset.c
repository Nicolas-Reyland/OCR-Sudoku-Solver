#include "dataset.h"
#include <stdlib.h>

nn_DataSet* _nn_createDataSet(nn_Data* trainData, nn_Data* testData)
{
	nn_DataSet* set = mem_malloc(sizeof(nn_DataSet));
	set->train 		= trainData;
	set->test 		= testData;

	return set;
}

void _nn_freeDataSet(nn_DataSet* dataSet)
{
	_nn_freeData(dataSet->train,true);
	_nn_freeData(dataSet->test,true);
	mem_free(dataSet);
}
