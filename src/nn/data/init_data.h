// init_data.h

#ifndef INIT_DATA_H
#define INIT_DATA_H

#define NB_DOUBLE_BITS 64

#include "data.h"
#include "nn/utils/structs/dataset.h"
#include "in_out_tuple.h"
#include "utils/verbosity/verbose.h"
#include <stdlib.h>
#include <string.h>

//public functions
nn_Data* nn_DataLoadRaw(char* input_path, char* output_path, nn_ShapeDescription* description, bool verb_mode);
nn_DataSet* nn_loadData(char* data_dir_path, nn_ShapeDescription* description, bool verb_mode);

#endif
