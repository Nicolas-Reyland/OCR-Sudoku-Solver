// derivation.c
#include "derivation.h"

double dSigmoid(double x);
double dRelu(double x);


/* Derivative Activation */
double _nn_derivativeActivation(double x, activation activation)
{
  double output;
	switch(activation)
  {
      case SIGMOID:
          return dSigmoid(x);
      case RELU:
          return dRelu(x);
      case SOFTMAX:
          return dSigmoid(x); //there is no derivative for softmax
      default:
          fprintf(stderr, 
          "derivativeActivation: Unrecognised activation function: %d\n",
           activation);
			     exit(EXIT_FAILURE);
		       break;
  }
	return output;
}

double dSigmoid(double x)
{
    return (exp(x))/((1+exp(x))*(1+exp(x)));
}

double dRelu(double x)
{
    return x < 0 ? 0 : 1;
}
