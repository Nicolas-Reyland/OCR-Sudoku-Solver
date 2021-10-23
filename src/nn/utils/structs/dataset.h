// dataset.h

#ifndef NN_DATASET_H
#define NN_DATASET_H

#include "nn/data/data.h"

typedef struct nn_DataSet
{
    nn_Data* train; //classical proportions 70% of whole data we got
    nn_Data* test;  //classical proportions 30% of whole data we got
} nn_DataSet;

nn_DataSet* nn_createDataSet(nn_Data* trainData, nn_Data* testData);
void _nn_freeDataSet(nn_DataSet* dataSet);


#endif
