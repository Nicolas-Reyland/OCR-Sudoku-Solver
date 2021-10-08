// data.h

#ifndef NN_DATA_H
#define NN_DATA_H

#include "data_collection.h"
#include "data_tuple.h"
// code comes here
typedef struct nn_Data //needs to be defined as a pointer please
{
    nn_DataCollection* data_collection;

    nn_DataTuple (*splitTrainTest) (struct nn_Data* data,
     int splittingPercentage);
} nn_Data;

nn_Data* createData(nn_DataCollection* collection);
void freeData(nn_Data* data);

#endif
