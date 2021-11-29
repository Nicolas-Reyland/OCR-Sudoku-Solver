#ifndef SDL_OPERATIONS_H
#define SDL_OPERATIONS_H

#include <SDL2/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <err.h>

SDL_Surface* cv_load_image(char *path);
Uint32 cv_get_pixel(SDL_Surface *surface, unsigned x, unsigned y);

#endif