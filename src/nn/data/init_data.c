// init_data.c

#include "init_data.h"

double _convertStringToDouble(char* string);
char _readFile(FILE* file, nn_ShapeDescription* description, double* values);


nn_Data* nn_DataLoadRaw(char* input_path, char* output_path, nn_ShapeDescription* description)
{
    FILE* input_file = fopen(input_path,"r+");
    FILE* output_file = fopen(output_path, "r+");

    if(input_file == NULL)
    {
        verbose("ERROR 404: %s, the file does not exist. Exiting...\n", *input_path);
        exit(EXIT_FAILURE);
    }
    if(output_file == NULL)
    {
        verbose("ERROR 404: %s, the file does not exist. Exiting...\n", *output_path);
        exit(EXIT_FAILURE);
    }
    iot_linked_list* data_list = init_iot_linked_list();

    // was "%ld %ld %ld %ld %d". brought back to "%ld %ld %ld %ld". hope its ok
    fscanf(input_file,"%ld %ld %ld %ld",&(description->dims), &(description->x),
    &(description->y), &(description->z));

    nn_ShapeDescription output_description;

    fscanf(output_file,"%ld %ld %ld %ld", 
    &output_description.dims, 
    &output_description.x, &output_description.y, &output_description.z);

    char cursorInput = fgetc(input_file);
    while(cursorInput != '\n')
        cursorInput = fgetc(input_file); //reading line to end for input file

    cursorInput = fgetc(output_file);
    while(cursorInput != '\n')
        cursorInput = fgetc(output_file); //same for output file

    do
    {
        double* input_values = NULL;
        double* output_values = NULL;
        cursorInput = _readFile(input_file, description, input_values);

        //we don't need to read the cursor of output_file
        //since normally it should have the same nb of lines than
        //input_file (for safety later, make a private function that test it)
        _readFile(output_file,&output_description,output_values);
        
        nn_Sample* input    = createSample(*description,input_values,
            description->x*description->y*description->z);
        nn_Sample* output   = createSample(*description,output_values,
            output_description.x*output_description.y*output_description.z);

        nn_InOutTuple* tuple = createInOutTuple(input,output);

        data_list->append_value(data_list,tuple);
    } while (cursorInput != EOF);

    fclose(input_file);
    fclose(output_file);

    return _nn_createData(_nn_loadDataCollection(data_list));
}



char _readFile(FILE* file, nn_ShapeDescription* description, double* values)
{
    size_t j = 0; //cursor in our double array;

    //the size of the double array
    size_t value_size = description->x*description->y*description->z;

    //normally, if we do the right things, then we define the dimensions
    //that are not used to 1, so that it doesn't break the malloc sizing lol
    values = mem_malloc(sizeof(double)*value_size);

    char car = fgetc(file);
    while(car != '\n' && car != EOF)
    {
        char* string = mem_malloc(sizeof(char) * NB_DOUBLE_BITS);
        size_t i = 0; //cursor in our string



        if(car != ' ')
        {
            //if we have allocated enough space to the string
            if(i < NB_DOUBLE_BITS)
                string[i] = car;
            else                    //dear God
            {
                verbose("Error at loading data, string buffer not enough big to\
                 store current data... exiting\n");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        else //if we hit a space, than we convert the string
        // and store the value in the array
        {
            values[j] = _convertStringToDouble(string);
        }
        car = fgetc(file);
    }
    return car;
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
