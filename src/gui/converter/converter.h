// converter.h

#ifndef CONVERTER_H
#define CONVERTER_H

#include "sdl_operations.h"
#include "utils/cell/cell.h"
#include "utils/mem/mem-management.h"
#include "utils/verbosity/verbose.h"
#include <dirent.h>
#include <string.h>

#define DT_REG 8
#define IMG_SIZE 784

void converter(char *path, double **converted_cells, Cell *cells_position);

unsigned int count_files(char *path);

#endif
