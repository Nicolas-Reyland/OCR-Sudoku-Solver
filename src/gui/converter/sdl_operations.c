// sdl_operations.c

#include "sdl_operations.h"

SDL_Surface *cv_load_image(char *path) {
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

static inline Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y) {
    int bpp = surf->format->BytesPerPixel;
    return (Uint8 *)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 cv_get_pixel(SDL_Surface *surface, unsigned x, unsigned y) {
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel) {
    case 1:
        return *p;

    case 2:
        return *(Uint16 *)p;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *(Uint32 *)p;
    }

    return 0;
}