#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operations.h"
#include "tuple.h"
#include "matrix.h"

Tuple* findhomography(Tuple src, size_t size);

SDL_Surface* transformimage(Tuple *transform, SDL_Surface *image, size_t size);
