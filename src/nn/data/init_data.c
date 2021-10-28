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
  strcat(test_output_path, "train.out");
  //
  verbose("train inputs:  %s", train_input_path);
  verbose("train outputs: %s", train_output_path);
  verbose("test inputs:   %s", test_input_path);
  verbose("test outputs:  %s", test_output_path);
  // load the paths
  nn_Data* train_data = nn_loadSingleDataInputOutput(train_input_path, train_output_path, description, verb_mode);
  nn_Data* test_data = nn_loadSingleDataInputOutput(test_input_path, test_output_path, description, verb_mode);
  // create the dataset
  nn_DataSet* dataset = nn_createDataSet(train_data, test_data);
  return dataset;
}

char defineShapeDescription(nn_ShapeDescription* description, FILE* file);
bool _readLineInFile(FILE* file, size_t num_values, double* values);
void verifyListCompleteness(iot_ll_node* node, size_t length);

nn_Data* nn_loadSingleDataInputOutput(char* input_path, char* output_path, nn_ShapeDescription* description, bool verb_mode)
{
    FILE* input_file = fopen(input_path,"r+");
    FILE* output_file = fopen(output_path, "r+");

    if (input_file == NULL)
    {
        fprintf(stderr, "ERROR 404: \"%s\", the file does not exist. Exiting...\n", input_path);
        exit(EXIT_FAILURE);
    }
    if (output_file == NULL)
    {
        fprintf(stderr, "ERROR 404: \"%s\", the file does not exist. Exiting...\n", output_path);
        exit(EXIT_FAILURE);
    }
    iot_linked_list* data_list = init_iot_linked_list();

    nn_ShapeDescription output_description = emptyShapeDescription();

    char cursorInput =  defineShapeDescription(description, input_file);
                        defineShapeDescription(&output_description, output_file);

    size_t input_size   =   description->range;
    size_t output_size  =   output_description.range;

    verifyListCompleteness(data_list->head, data_list->length);

    // Note: lines_read is only incremented if the VERBOSE is on
    // Else, it will remain 1
    size_t lines_read = 0;
    // Same Note as lines_read
    size_t next_verbose_print = 100;

    do
    {
        // verbose print
        // better to check now for VERBOSE, bc we don't want to enter calculate lines_read == next_verbose_print
        // for nothing
        if (verb_mode && ++lines_read == next_verbose_print) {
            verbose("Lines read: %ld", lines_read);
            next_verbose_print += 100;
        }
        //normally, if we do the right things, then we define the dimensions
        //that are not used to 1, so that it doesn't break the malloc sizing lol
        double* input_values    = mem_calloc(input_size, sizeof(double));
        double* output_values   = mem_calloc(output_size, sizeof(double));
        if (!_readLineInFile(input_file, input_size, input_values)) {
          mem_free(input_values);
          mem_free(output_values);
          break;
        }

        //we don't need to read the cursor of output_file
        //since normally it should have the same nb of lines than
        //input_file (for safety later, make a private function that test it)
        if (!_readLineInFile(output_file, output_size, output_values)) {
          // print before freeing, in case we have a problem with the mem_free
          // the error msg now gets printed anyways
          fprintf(stderr, "Not same number of lines in input file and output file. Exiting...\n");
          mem_free(input_values);
          mem_free(output_values);
          exit(EXIT_FAILURE);
          return NULL;
        }

        //
        nn_Sample* input    = createSample(*description,input_values,
            description->range);
        nn_Sample* output   = createSample(*description,output_values,
            output_description.range);

        nn_InOutTuple* tuple = createInOutTuple(input,output);
        data_list->append_value(data_list,tuple);
        verifyListCompleteness(data_list->head,data_list->length);
    } while (cursorInput != EOF);

    fclose(input_file);
    fclose(output_file);

    return _nn_createData(_nn_loadDataCollection(data_list));
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

    //verbose("line: %p", line_start);
    for (size_t i = 0; i < num_values; i++) {
        //verbose("start: %ld", i);
        // verify allocation of values[j]
        if(&values[i] == NULL)
        {
            verbose("Getting segfault because values[%ld] does not exist.", i);
            exit(EXIT_FAILURE);
        }
        // parse next double
        value = strtod(line_start, &read_end);
        line_start = read_end;
        // asign value
        values[i] = value;
        //verbose("end: %lf", value);
    }

    // this value has been allocated by 'getline'
    free(line_begin_ptr);

    return true;
}

char defineShapeDescription(nn_ShapeDescription* description, FILE* file)
{
    //we define the shape description by reading the file
    int matched_values = fscanf(file,"%ld %ld %ld %ld",&(description->dims), &(description->x),
    &(description->y), &(description->z));
    // re-calculate range
    description->range = description->x * description->y * description->z;
    // veriify that the line was well matched
    if (matched_values != 4) {
      fprintf(stderr, "Could not match all the values of the (first) description line. Values matched: %d\n", matched_values);
      exit(EXIT_FAILURE);
    }
    char cursor = fgetc(file);
    while(cursor != '\n')
        cursor = fgetc(file); //reading to end of line

    return cursor;
}

void verifyListCompleteness(iot_ll_node* node, size_t length)
{
    if(node == NULL)
    {
        if(length == 0)
            return;
        verbose("Linked list is not complete. There is too much element linked...");
        exit(EXIT_FAILURE);
    }
    verifyListCompleteness(node->next,length-1);
}
