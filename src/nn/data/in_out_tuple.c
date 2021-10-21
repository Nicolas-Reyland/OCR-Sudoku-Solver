// in_out_tuple.c

#include "in_out_tuple.h"

void _nn_printTuple(nn_InOutTuple* tuple)
{
    verbose("number of input values: %ld", tuple->input->num_values);
    verbose("number of output values: %ld", tuple->output->num_values);

    verbose("Input:");
    tuple->input->print(tuple->input);
    
    verbose("Output expected:");
    tuple->output->print(tuple->output);
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
    freeSample(tuple->input);
    freeSample(tuple->output);
    mem_free(tuple);
}
