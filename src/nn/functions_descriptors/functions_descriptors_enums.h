// functions_descriptor_enums.h

#ifndef NN_FUNCTIONS_DESCRIPTOR_ENUMS_H
#define NN_FUNCTIONS_DESCRIPTOR_ENUMS_H

typedef enum losses
{
	CATEGORICALCROSSENTROPY,
	BINARYCROSSENTROPY,
	MEANSQUAREDERROR
} losses;

typedef enum activation
{
	NO_ACTIVATION, // e.g. for the input layer
	SIGMOID,
	RELU,
	LEAKY_RELU,
	SOFTMAX, // output layer only
	TANH
} activation;

typedef enum optimizer
{
	RMSPROP,
	ADAM
} optimizer;

#endif
