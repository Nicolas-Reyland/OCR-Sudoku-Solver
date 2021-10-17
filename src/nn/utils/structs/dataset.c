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
	
	if(dataSet == NULL)
    {
        verbose("freeDataSet: dataSet is null.");
        exit(EXIT_FAILURE);
    }
	verbose("Freeing test data...");
	_nn_freeData(dataSet->test,true);
	verbose("Test data freed !");

	verbose("Freeing train data...");
	_nn_freeData(dataSet->train,true);
	verbose("Train data freed !");
	
	verbose("Freeing dataset...");
	mem_free(dataSet);
	verbose("Dataset freed !");
}
