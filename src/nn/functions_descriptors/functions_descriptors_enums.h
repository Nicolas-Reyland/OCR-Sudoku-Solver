// functions_descriptor_enums.h

#ifndef NN_FUNCTIONS_DESCRIPTOR_ENUMS_H
#define NN_FUNCTIONS_DESCRIPTOR_ENUMS_H

typedef enum losses
{
	CATEGORICALCROSSENTROPY,
	BINARYCROSSENTROPY,
	SPARSECATEGORICALCROSSENTROPY
} losses;

typedef enum activation
{
	SIGMOID,
	RELU,
	SOFTMAX,
	TANH
} activation;

typedef enum optimizer
{
	RMSPROP,
	ADAM
} optimizer;

#endif
