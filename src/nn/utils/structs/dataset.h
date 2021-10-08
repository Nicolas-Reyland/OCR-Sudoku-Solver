// dataset.h

#ifndef NN_DATASET_H
#define NN_DATASET_H

#include "../data/data.h"

typedef struct nn_DataSet
{
    nn_Data* train; //should normally represent 70% of whole data we got
    nn_Data* test;  //should normally represent 30% of whole data we got
}nn_DataSet;

nn_DataSet* createDataSet(nn_Data* trainData, nn_Data* testData);
void freeDataSet(nn_DataSet* dataSet);


#endif
