#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <stdio.h>
#include <stdlib.h>
#include "tuple.h"

void swap(CCTuple *a, CCTuple *b);
void heapify(CCTuple arr[], size_t size, int i);
void heapSort(CCTuple arr[], size_t size);
void printCCTupleArr(CCTuple arr[], size_t size);

#endif
