#ifndef SEGEMNETATION_H
#define SEGEMENTATION_H

#include <SDL2/SDL.h>
#include "pixel_operations.h"
#include "../../utils/mem/linked_list.h"
#include "set.h"
#include "tuple.h"

Tuple* twopassSegmentation(SDL_Surface *image, SDL_Surface* segmap);

#endif