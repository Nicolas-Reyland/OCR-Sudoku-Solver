// in_out_tuple.c

#include "in_out_tuple.h"

void _nn_printTuple(nn_InOutTuple* tuple)
{
    printf("number of input values: %ld\n", tuple->input->nb_values);
    printf("number of output values: %ld\n", tuple->output->nb_values);

    printf("Input:\n");
    tuple->input->print(tuple->input);
    
    printf("Output expected:\n");
    tuple->output->print(tuple->output);
    printf("pointer address: %p\n",tuple);
    printf("\n");
}


nn_InOutTuple* createInOutTuple(nn_Sample* input, nn_Sample* output)
{
    nn_InOutTuple* tuple = mem_malloc(sizeof(nn_InOutTuple));

    tuple->input        = input;
    tuple->output       = output;
    tuple->printTuple   = &_nn_printTuple;
    return tuple;
}

void freeInOutTuple(nn_InOutTuple* tuple)
{
    if(tuple == NULL)
    {
        verbose("freeInOutTuple: tuple is null.");
        exit(EXIT_FAILURE);
    }
    printf("pointer tuple address: %p\n",tuple);
    freeSample(tuple->input);
    freeSample(tuple->output);
    mem_free(tuple);
}
