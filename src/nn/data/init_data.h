// init_data.h

#ifndef INIT_DATA_H
#define INIT_DATA_H

#define NB_DOUBLE_BITS 64

#include "data.h"
#include "in_out_tuple.h"
#include "nn/utils/structs/dataset.h"
#include "utils/verbosity/progressbar.h"
#include "utils/verbosity/verbose.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// public functions
nn_Data nn_loadSingleDataInputOutput(char *input_path, char *output_path,
                                     bool verb_mode, const char *verb_string);
nn_DataSet nn_loadDataSet(char *data_dir_path, bool verb_mode);
nn_DataSet nn_loadTestOnlyDataSet(char *data_dir_path, bool verb_mode);

#endif
