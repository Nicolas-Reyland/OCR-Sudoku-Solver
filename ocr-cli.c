// ocr-cli.c

#include "nn/nn.h"
#include <string.h>

double* convert_input(char* input_data);

int main(int argc, char** argv)
{
	setVerbose(false);
	if (argc != 3)
		err_verbose_exit("Usage: ./ocr-cli.c.out model-name input-data");
	char model_path[255];
	strcpy(model_path, "save/");
	strcat(model_path, argv[1]);
	char* input_data = argv[2];

	// init random & memory tracking
	nn_initRandom();
	initMemoryTracking();

	// malloc model
	nn_Model* model = nn_loadModel(model_path);
	
	// transform the input data
	double* input = convert_input(input_data);
	
	// use the model
	double* prediction = model->use(model, input);
	
	// find prediction
	size_t max_i = 0;
	for (size_t i = 1; i < 9; i++)
		if (prediction[i] > prediction[max_i])
			max_i = i;
	
	// output prediction to stdout
	printf("%lu\n", max_i + 1);

	mem_freeGPL(false);

	return 0;
}

double* convert_input(char* input_data)
{
	double* input = mem_calloc(784, sizeof(double));
	for (size_t i = 0; i < 196; i++) {
		char c = input_data[i],
			 x;
		if (c <= '9')
			x = c - '0';
		else
			x = 10 + c - 'a';
		input[4 * i]     = x & 1;
		input[4 * i + 1] = x & 2;
		input[4 * i + 2] = x & 4;
		input[4 * i + 3] = x & 8;
	}
	return input;
}

