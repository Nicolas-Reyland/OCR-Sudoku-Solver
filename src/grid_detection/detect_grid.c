#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

void segmentation(SDL_Surface* image, SDL_Surface* segmap);

void propagate(SDL_Surface* image, SDL_Surface* segmap, int segval, size_t x, size_t y);

void colorimage(SDL_Surface* image, SDL_Surface* segmap);



void detect_grid(SDL_Surface* image)
{
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    segmentation(image, segmap);
    colorimage(image, segmap);
}

void segmentation(SDL_Surface* image, SDL_Surface* segmap)
{
    int segval = 1;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h ; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if (color == 0)
            {
                Uint32 p = get_pixel(segmap, x, y);
                if(p == 0)
                {
                    propagate(image, segmap, segval, x, y);
                    segval+=1;
                }
            }   
        }
    }
}

void propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y)
{
    Uint8 color;
    Uint32 pix = get_pixel(image, x, y);
    SDL_GetRGB(pix, image->format, &color, &color, &color);
    Uint32 p = get_pixel(segmap, x, y);
    if(p != 0 || color == 0)
    {
        return;
    }
    put_pixel(segmap, x, y, segval);
    if(x < (size_t)image->w-1)
    {
        propagate(image, segmap, segval, x+1, y);
    }
    if(x > 0)
    {
        propagate(image, segmap, segval, x-1, y);
    }
    if(y < (size_t)image->h-1)
    {
        propagate(image, segmap, segval, x, y+1);
    }
    if(y > 0)
    {
        propagate(image, segmap, segval, x, y-1);
    }
}

void colorimage(SDL_Surface *image, SDL_Surface* segmap)
{
    Uint8 r,g,b;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 val = get_pixel(segmap, x, y);
            r = 3*val%256;
            g = 5*val%256;
            b = 7*val%256;
            put_pixel(image, x, y, SDL_MapRGB(image->format, r, g, b));
        }
    }
}