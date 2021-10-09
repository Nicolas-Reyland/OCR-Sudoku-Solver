#include "init_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double _convertStringToDouble(char* string);



nn_Data* nn_Data_load_raw(char* path, nn_ShapeDescription* description)
{
    FILE* input_file = fopen(path,"r+");

    if(input_file == NULL)
    {
        printf("%s, the file does not exist. Exiting...\n", &path);
        exit(EXIT_FAILURE);
    }
    
    fscanf(input_file,"%d %d %d %d %d",&description->dims, &description->x, 
    &description->y, &description->z);



    char car = fgetc(input_file);
    while(car != '\n')
        car = fgetc(input_file); //reading line to end 

    size_t j = 0; //cursor in our double array;

    //the size of the double array
    size_t value_size = description->x*description->y*description->z; 

    //normally, if we do the right things, then we define the dimensions
    //that are not used to 1, so that it doesn't break the malloc sizing lol
    double* value = malloc(sizeof(double)*value_size); 
    while(car != EOF)
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
            value[j] = _convertStringToDouble(string);
        }
        car = fgetc(input_file);
    }
    fclose(input_file);
}

double _convertStringToDouble(char* string)
{
    //more on the next episode
    return 0;
}