// converter.h

#ifndef CONVERTER_H
#define CONVERTER_H

#include <dirent.h>
#include <string.h>
#include "utils/verbosity/verbose.h"
#include "sdl_operations.h"
#include "utils/cell/cell.h"

#define DT_REG 0
#define IMG_SIZE 784

void converter(char* path, double** converted_cells, Cell** cells_position);

#endif