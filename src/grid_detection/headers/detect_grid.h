#ifndef DETECT_GRID_H
#define DETECT_GRID_H

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"
#include "grid_utils.h"
#include "segmentation.h"
#include "tuple.h"

void detect_grid(const char* file);
size_t findgrid(CCTuple *histo, size_t histo_size, size_t* coords[], SDL_Surface** surf[]);
int get_distance(int X1, int Y1, int X2, int Y2);

#endif