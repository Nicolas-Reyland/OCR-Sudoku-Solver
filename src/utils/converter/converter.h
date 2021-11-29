#ifndef CONVERTER_H
#define CONVERTER_H

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "sdl_operations.h"
#include "../cell/cell.h"

#define IMG_SIZE 784

void converter(char* path, double** converted_cells, Cell** cells_position);

#endif