#include "tonumber.h"

int toNumber(double* input, size_t size)
{
	// The function will send a number =/= 0 only if the 
	// network is very sure about the result it is outputing

	int index = 0;
	double value = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (input[i] > value && input[i] > THRESHOLD)
		{
			index = (int)i + 1;
			value = input[i];
		}
	}
	return index;
	
}