// init_data.c

#include "init_data.h"

// declare get_line function here to prevent from a compilation warning
ssize_t getline(char **restrict lineptr, size_t *restrict n,
		FILE *restrict stream);

nn_DataSet* nn_loadDataSet(char* data_dir_path, nn_ShapeDescription* description, bool verb_mode)
{
	// alloc path strings
	char train_input_path[255];
	char test_input_path[255];
	char train_output_path[255];
	char test_output_path[255];
	// concat paths
	strcpy(train_input_path, data_dir_path);
	strcat(train_input_path, "train.in");
	//
	strcpy(train_output_path, data_dir_path);
	strcat(train_output_path, "train.out");
	//
	strcpy(test_input_path, data_dir_path);
	strcat(test_input_path, "test.in");
	//
	strcpy(test_output_path, data_dir_path);
	strcat(test_output_path, "test.out");
	//
	nn_verbose("train inputs:  %s", train_input_path);
	nn_verbose("train outputs: %s", train_output_path);
	nn_verbose("test inputs:   %s", test_input_path);
	nn_verbose("test outputs:  %s", test_output_path);
	// load the paths
	nn_Data* train_data = nn_loadSingleDataInputOutput(train_input_path, train_output_path, description, verb_mode, "Loading 'Train' dataset");
	nn_Data* test_data = nn_loadSingleDataInputOutput(test_input_path, test_output_path, description, verb_mode, "Loading 'Test' dataset");
	// create the dataset
	nn_DataSet* dataset = nn_createDataSet(train_data, test_data);
	return dataset;
}

nn_DataSet* nn_loadTestOnlyDataSet(char* data_dir_path, nn_ShapeDescription* description, bool verb_mode)
{
	nn_verbose("/!\\ Warning: only loading the test dataset");
	// alloc path strings
	char test_input_path[255];
	char test_output_path[255];
	// concat paths
	strcpy(test_input_path, data_dir_path);
	strcat(test_input_path, "test.in");
	//
	strcpy(test_output_path, data_dir_path);
	strcat(test_output_path, "test.out");
	//
	nn_verbose("test inputs:   %s", test_input_path);
	nn_verbose("test outputs:  %s", test_output_path);
	// load the paths
	nn_Data* train_data = NULL;
	nn_Data* test_data = nn_loadSingleDataInputOutput(test_input_path, test_output_path, description, verb_mode, "Loading 'Test' dataset");
	// create the dataset
	nn_DataSet* dataset = nn_createDataSet(train_data, test_data);
	return dataset;
}

char defineShapeDescription(nn_ShapeDescription* description, size_t* num_tuples, FILE* file);
bool _readLineInFile(FILE* file, size_t num_values, double* values);

nn_Data* nn_loadSingleDataInputOutput(char* input_path, char* output_path, nn_ShapeDescription* description, bool verb_mode, const char* verb_string)
{
    FILE* input_file = fopen(input_path,"r+");
    FILE* output_file = fopen(output_path, "r+");

    if (input_file == NULL)
    {
        nn_err_nn_verbose_exit("ERROR 404: \"%s\", the file does not exist. Exiting...\n", input_path);
    }
    if (output_file == NULL)
    {
        nn_err_nn_verbose_exit("ERROR 404: \"%s\", the file does not exist. Exiting...\n", output_path);
    }

    nn_ShapeDescription output_description = nn_emptyShapeDescription();

	size_t num_tuples = 0;
    defineShapeDescription(description, &num_tuples, input_file);
	size_t num_tuples_output = 0;
    defineShapeDescription(&output_description, &num_tuples_output, output_file);
	if (num_tuples != num_tuples_output) {
		nn_err_nn_verbose_exit("Not same amount of data described in input/output: %lu != %lu", num_tuples, num_tuples_output);
	}

	// allocate memory for input output tuples
	nn_InOutTuple** iot_array  = mem_malloc(num_tuples * sizeof(nn_InOutTuple*));

    size_t input_data_size   =   description->range;
    size_t output_data_size  =   output_description.range;

	// Progress bar
	ProgressBar data_load_bar;
	if (verb_string != NULL)
		data_load_bar = createProgressBar(verb_string, 0, num_tuples, DEFAULT_PROGRESSBAR_WIDTH);
	else
		data_load_bar = createProgressBar(NULL, 0, num_tuples, DEFAULT_PROGRESSBAR_WIDTH);

    size_t  next_nn_verbose_print = 100,
			next_nn_verbose_print_step = num_tuples / 100;

    for (size_t data_index = 0; data_index < num_tuples; data_index++)
    {
        // nn_verbose print
        // better to check now for VERBOSE, bc we don't want to enter calculate data_index == next_nn_verbose_print
        // for nothing
        if (verb_mode && data_index == next_nn_verbose_print) {
            //nn_verbose("Lines read: %ld", data_index);
			updateProgressBar(&data_load_bar, data_index);
            next_nn_verbose_print += next_nn_verbose_print_step;
        }
        //normally, if we do the right things, then we define the dimensions
        //that are not used to 1, so that it doesn't break the malloc sizing lol
        double* input_values    = mem_calloc(input_data_size, sizeof(double));
        double* output_values   = mem_calloc(output_data_size, sizeof(double));
        if (!_readLineInFile(input_file, input_data_size, input_values)) {
          mem_free(input_values);
          mem_free(output_values);
          break;
        }

        //we don't need to read the cursor of output_file
        //since normally it should have the same nb of lines than
        //input_file (for safety later, make a private function that test it)
        if (!_readLineInFile(output_file, output_data_size, output_values)) {
          // print before freeing, in case we have a problem with the mem_free
          // the error msg now gets printed anyways
          mem_free(input_values);
          mem_free(output_values);
          nn_err_nn_verbose_exit("Not same number of lines in input file and output file. Exiting...");
          return NULL;
        }

        //
        nn_Sample* input    = createSample(
			*description,
			input_values,
            description->range
		);
        nn_Sample* output   = createSample(
			*description,
			output_values,
            output_description.range
		);

        iot_array[data_index] = _nn_createInOutTuple(input, output);
    } // while (cursorInput != EOF);

	endProgressBar(&data_load_bar);

    fclose(input_file);
    fclose(output_file);

    return _nn_createData(_nn_loadDataCollection(iot_array, num_tuples));
}


bool _readLineInFile(FILE* file, size_t num_values, double* values)
{
    // read the next line
    char* line_start = NULL;
    size_t line_buffer_size = 0;
    ssize_t chars_read = getline(&line_start, &line_buffer_size, file);
    char* line_begin_ptr = line_start;

    // if we have reached the end, exit the function
    if (line_start == NULL || chars_read < 1)
      return false;

    // setup variables for reading the line and the values in it
    double value = 0.0/0.0; // nan
    char* read_end = NULL;

    //

    //nn_verbose("line: %p", line_start);
    for (size_t i = 0; i < num_values; i++) {
        //nn_verbose("start: %ld", i);
        // verify allocation of values[j]
        if(&values[i] == NULL)
        {
            nn_err_nn_verbose_exit("Getting segfault because values[%ld] does not exist.", i);
        }
        // parse next double
        value = strtod(line_start, &read_end);
        line_start = read_end;
        // asign value
        values[i] = value;
        //nn_verbose("end: %lf", value);
    }

    // this value has been allocated by 'getline'
    free(line_begin_ptr);

    return true;
}

char defineShapeDescription(nn_ShapeDescription* description, size_t* num_tuples, FILE* file)
{
    //we define the shape description by reading the file
    int matched_values = fscanf(file,"%ld %ld %ld %ld %ld",
		num_tuples,
		&description->dims,
		&description->x,
    	&description->y,
		&description->z
	);
    // re-calculate range
    description->range = description->x * description->y * description->z;
    // veriify that the line was well matched
    if (matched_values != 5) {
    	nn_err_nn_verbose_exit("Could not match all the values of the (first) description line. Values matched: %d\n", matched_values);
    }
    char cursor = fgetc(file);
    while(cursor != '\n')
        cursor = fgetc(file); //reading to end of line

    return cursor;
}
