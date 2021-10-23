#ifndef PIXEL_OPERATIONS_H
#define PIXEL_OPERATIONS_H

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

#endif