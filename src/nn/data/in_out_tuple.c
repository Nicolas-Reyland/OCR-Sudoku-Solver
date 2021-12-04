// in_out_tuple.c

#include "in_out_tuple.h"

static void _nn_printTuple(nn_InOutTuple tuple)
{
    nn_verbose("number of input values: %ld", tuple.input.num_values);
    nn_verbose("number of output values: %ld", tuple.output.num_values);

    nn_verbose("Input:");
    tuple.input.print(tuple.input);
    nn_verbose("Output expected:");
    tuple.output.print(tuple.output);
}

nn_InOutTuple _nn_createInOutTuple(nn_Sample input, nn_Sample output)
{
    nn_InOutTuple tuple; // = mem_malloc(sizeof(nn_InOutTuple));

    tuple.input        = input;
    tuple.output       = output;
    tuple.printTuple   = &_nn_printTuple;
    return tuple;
}
