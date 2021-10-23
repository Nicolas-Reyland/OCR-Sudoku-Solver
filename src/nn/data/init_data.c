// init_data.c

#include "init_data.h"

double _convertStringToDouble(char* string);
char defineShapeDescription(nn_ShapeDescription* description, FILE* file);
char _readFile(FILE* file, nn_ShapeDescription* description, double* values);

void verifyListCompleteness(iot_ll_node* node, size_t length);




nn_Data* nn_DataLoadRaw(char* input_path, char* output_path, nn_ShapeDescription* description)
{
    FILE* input_file = fopen(input_path,"r+");
    FILE* output_file = fopen(output_path, "r+");

    if (input_file == NULL)
    {
        verbose("ERROR 404: %s, the file does not exist. Exiting...\n", *input_path);
        exit(EXIT_FAILURE);
    }
    if (output_file == NULL)
    {
        verbose("ERROR 404: %s, the file does not exist. Exiting...\n", *output_path);
        exit(EXIT_FAILURE);
    }
    iot_linked_list* data_list = init_iot_linked_list();

    nn_ShapeDescription output_description = emptyShapeDescription();
                        
    char cursorInput =  defineShapeDescription(description, input_file);
                        defineShapeDescription(&output_description, output_file);

    size_t input_size   =   description->x * description->y * description->y;
    size_t output_size  =   output_description.x * output_description.y *
                            output_description.z;
    
    verifyListCompleteness(data_list->head,data_list->length);

    do
    {
        //normally, if we do the right things, then we define the dimensions
        //that are not used to 1, so that it doesn't break the malloc sizing lol
        double* input_values    = mem_calloc(input_size, sizeof(double));
        double* output_values   = mem_calloc(output_size, sizeof(double));
        cursorInput = _readFile(input_file, description, input_values);

        //we don't need to read the cursor of output_file
        //since normally it should have the same nb of lines than
        //input_file (for safety later, make a private function that test it)
        _readFile(output_file, &output_description, output_values);
        nn_Sample* input    = createSample(*description,input_values,
            description->x*description->y*description->z);
        nn_Sample* output   = createSample(*description,output_values,
            output_description.x*output_description.y*output_description.z);

        nn_InOutTuple* tuple = createInOutTuple(input,output);
        data_list->append_value(data_list,tuple);
        verifyListCompleteness(data_list->head,data_list->length);
    } while (cursorInput != EOF);

    fclose(input_file);
    fclose(output_file);
    
    return _nn_createData(_nn_loadDataCollection(data_list));
}



char _readFile(FILE* file, nn_ShapeDescription* description, double* values)
{
    size_t j = 0; //cursor in our double array;

    char car;

    //initializing our string buffer
    char* string = mem_malloc(sizeof(char) * NB_DOUBLE_BITS);
    size_t i = 0; //cursor in our string 
    
    bool continue_reading;
    do
    {
        car = fgetc(file);
        continue_reading = (car != EOF && car != '\n');
        if (car == '.' || ('0'<=car && car <= '9'))
        {
            string[i] = car;
            i++;
        }
        else
        {
            if(&values[j] == NULL)
            {
                verbose("Getting segfault because values[%ld] does not exist.",j);
                exit(EXIT_FAILURE);
            }
            values[j] = _convertStringToDouble(string);
            if(continue_reading)
            {
                string = mem_malloc(sizeof(char) * NB_DOUBLE_BITS);
                i=0;
            }
            j++;
        }
    } while (continue_reading);
    //just checking if we arrived at the end of file...
    char car_after = fgetc(file);
    ungetc(car_after,file);
    return car_after == EOF ? car_after : car;
}

/// <Summary>
/// convert a string into a double value then frees the string
/// <Summary/>
double _convertStringToDouble(char* string)
{
    double value = strtod(string,NULL);

    mem_free(string);
    return value;
}

char defineShapeDescription(nn_ShapeDescription* description, FILE* file)
{
    //we define the shape description by reading the file
    int matched_values = fscanf(file,"%ld %ld %ld %ld",&(description->dims), &(description->x),
    &(description->y), &(description->z));
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