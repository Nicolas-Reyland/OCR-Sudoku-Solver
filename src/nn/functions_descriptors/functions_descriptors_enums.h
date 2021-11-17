// functions_descriptor_enums.h

#ifndef NN_FUNCTIONS_DESCRIPTOR_ENUMS_H
#define NN_FUNCTIONS_DESCRIPTOR_ENUMS_H

typedef enum losses
{
	NO_LOSS, // for safety
	CATEGORICALCROSSENTROPY,
	BINARYCROSSENTROPY,
	MEANSQUAREDERROR
} losses;

typedef enum activation
{
	NO_ACTIVATION, // for safety
	SIGMOID,
	RELU,
	LEAKY_RELU,
	SOFTMAX, // output layer only
	TANH
} activation;

typedef enum optimizer
{
	NO_OPTIMIZER, // for safety
	RMSPROP,
	ADAM
} optimizer;

#endif
