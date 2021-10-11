// init_data.c

#include "init_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double _convertStringToDouble(char* string);
char _readFile(FILE* file, nn_ShapeDescription* description, double* values);

<<<<<<< HEAD

nn_Data* nn_Data_load_raw(char* input_path, char* output_path, nn_ShapeDescription* description)
{
    FILE* input_file = fopen(input_path,"r+");
    FILE* output_file = fopen(output_path, "r+");
=======
nn_Data* nn_Data_load_raw(char* path, nn_ShapeDescription* description)
{
    FILE* input_file = fopen(path, "r+");
>>>>>>> 4e1ff95e62ae59a4a0b2f6d657a9256ab9f7d38b

    if(input_file == NULL)
    {
        printf("ERROR 404: %s, the file does not exist. Exiting...\n", *input_path);
        exit(EXIT_FAILURE);
    }
    if(output_file == NULL)
    {
        printf("ERROR 404: %s, the file does not exist. Exiting...\n", *output_path);
        exit(EXIT_FAILURE);
    }
<<<<<<< HEAD
    linked_list* data_list = init_linked_list();


    fscanf(input_file,"%d %d %d %d %d",&(description->dims), &(description->x), 
    &(description->y), &(description->z));



    char cursorInput = fgetc(input_file);
    while(cursorInput != '\n')
        cursorInput = fgetc(input_file); //reading line to end 

    do
    {
        double* input_values;
        double* output_values;
        cursorInput = readFile(input_file,description, input_values);
=======

    fscanf(input_file,"%d %d %d %d %d",&description->dims, &description->x,
    &description->y, &description->z);
>>>>>>> 4e1ff95e62ae59a4a0b2f6d657a9256ab9f7d38b

        //we don't need to read the cursor of output_file
        //since normally it should have the same nb of lines than
        //input_file (for safety later, make a private function that test it)
        readFile(output_file,description,output_values);

        nn_Sample* input    = createSample(*description,output_values);
        nn_Sample* output   = createSample(*description,input_values);

<<<<<<< HEAD
        nn_InOutTuple* tuple = createInOutTuple(input,output);
=======
    char car = fgetc(input_file);
    while(car != '\n')
        car = fgetc(input_file); //reading line to end
>>>>>>> 4e1ff95e62ae59a4a0b2f6d657a9256ab9f7d38b

        data_list->append_value(data_list,tuple);
    } while (cursorInput != EOF);

    fclose(input_file);
    fclose(output_file);

    return createData(loadDataCollection(data_list));
}



char _readFile(FILE* file, nn_ShapeDescription* description, double* values)
{
    size_t j = 0; //cursor in our double array;

    //the size of the double array
    size_t value_size = description->x*description->y*description->z;

    //normally, if we do the right things, then we define the dimensions
    //that are not used to 1, so that it doesn't break the malloc sizing lol
<<<<<<< HEAD
    values = malloc(sizeof(double)*value_size); 

    char car = fgetc(file);
    while(car != '\n' && car != EOF)
=======
    double* value = malloc(sizeof(double)*value_size);
    while(car != EOF)
>>>>>>> 4e1ff95e62ae59a4a0b2f6d657a9256ab9f7d38b
    {
        char* string = malloc(sizeof(char) * NB_DOUBLE_BITS);
        size_t i = 0; //cursor in our string



        if(car != ' ')
        {
            //if we have allocated enough space to the string
            if(i < NB_DOUBLE_BITS)
                string[i] = car;
            else                    //dear God
            {
                printf("Error at loading data, string buffer not enough big to\
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
<<<<<<< HEAD
    return car;
=======
    fclose(input_file);

    // return something here
>>>>>>> 4e1ff95e62ae59a4a0b2f6d657a9256ab9f7d38b
}

/// <Summary>
/// convert a string into a double value then frees the string
/// <Summary/>
double _convertStringToDouble(char* string)
{
    double value = strtod(string,NULL);
    free(string);
    return 0;
}
