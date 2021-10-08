#include "init_data.h"
#include <stdio.h>
#include <string.h>

nn_Data* nn_Data_load_raw(char* path)
{
    FILE* input_file = fopen(path,"r+");

    if(input_file == NULL)
    {
        printf("%s, the file does not exist. Exiting...", &path);
        exit(EXIT_FAILURE);
    }
    
}