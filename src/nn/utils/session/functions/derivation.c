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
          // there is no derivative for softmax
          // returning 1.0 so it has no impact (x * derivative = x * 1.0 = x)
          return 1.0;
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
    // the 'x' is already the output of a sigmoid function
    // and we don't want to calculate d_sigmoid(sigmoid(x)), but
    // d_sigmoid(x), thus returning ' x * (1.0 - x)'
    return x * (1.0 - x);
}

double dRelu(double x)
{
    return x < 0 ? 0 : 1;
}
