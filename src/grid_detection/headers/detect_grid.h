#ifndef DETECT_GRID_H
#define DETECT_GRID_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "pixel_operations.h"
#include "grid_utils.h"
#include "segmentation.h"
#include "tuple.h"

void detect_grid(const char* file);

#endif