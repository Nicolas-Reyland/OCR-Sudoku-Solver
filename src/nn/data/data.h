// data.h

#ifndef NN_DATA_H
#define NN_DATA_H

#include "data_collection.h"

typedef struct nn_DataTuple nn_DataTuple;

typedef struct nn_Data //needs to be defined as a pointer please
{
    nn_DataCollection* data_collection;

    nn_DataTuple (*splitTrainTest) (struct nn_Data*, int);
} nn_Data;

typedef struct nn_DataTuple
{
	nn_Data* data1;
	nn_Data* data2;
} nn_DataTuple;

nn_Data* createData(nn_DataCollection* collection);
void freeData(nn_Data* data);

#endif
