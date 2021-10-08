#include "dataset.h"
#include <stdio.h>
#include <stdlib.h>

nn_DataSet* createDataSet(nn_Data* trainData, nn_Data* testData)
{
	nn_DataSet* set = malloc(sizeof(nn_DataSet));
	set->train 		= trainData;
	set->test 		= testData;

	return set;
}

void freeDataSet(nn_DataSet* dataSet)
{
	freeData(dataSet->train);
	freeData(dataSet->test);
	free(dataSet);
}
