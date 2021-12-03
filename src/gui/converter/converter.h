// converter.h

#ifndef CONVERTER_H
#define CONVERTER_H

#include <dirent.h>
#include <string.h>
#include "utils/verbosity/nn_verbose.h"
#include "utils/mem/mem-management.h"
#include "sdl_operations.h"
#include "utils/cell/cell.h"

#define DT_REG 8
#define IMG_SIZE 784

void converter(char* path, double** converted_cells, Cell* cells_position);

unsigned int count_files(char* path);

#endif
