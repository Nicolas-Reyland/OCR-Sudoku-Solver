// node.c

#include "node.h"
#include <stdlib.h>
#include <stdio.h>

nn_Node* _nn_createNode(unsigned int num_weights)
{
  // malloc weights
  printf("   1/3 num weights: %d size:%d\n", num_weights, num_weights * sizeof(double));
  double *weights = calloc(num_weights, sizeof(double));
  printf("test\n");
  for (unsigned int i = 0; i < num_weights; i++) {
    weights[i] = 0.0;//getNormalizedRandomDouble();
  }
  double bias = getNormalizedRandomDouble();
  printf("   2/3 weights and bias malloced\n");
  // malloc struct
  nn_Node* node = malloc(sizeof(nn_Node));
  node->num_weights = num_weights;
  node->weights = weights;
  node->bias = bias;
  printf("   3/3 node created\n");
  return node;
}


//
