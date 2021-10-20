#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap);

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y);

void colorimage(SDL_Surface* image, SDL_Surface* segmap);

void remove_other_obj(SDL_Surface* image, SDL_Surface* segmap, size_t objval);

void find_extremity_coordinates(SDL_Surface* image, size_t* up, size_t* right, size_t* left, size_t* down);



void detect_grid(SDL_Surface* image)
{
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    size_t biggestObj = segmentation(image, segmap);
    colorimage(image, segmap);
    if(SDL_SaveBMP(image, "colorised.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    remove_other_obj(image, segmap, biggestObj);
    size_t top, right, left, bottom;
    find_extremity_coordinates(image, &top, &right, &left, &bottom);
    Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
    put_pixel(image, right, top, pix);
    put_pixel(image, left, top, pix);
    put_pixel(image, right, bottom, pix);
    put_pixel(image, left, bottom, pix);
}

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap)
{
    size_t max = 0;
    int labelmax = 0;
    int segval = 1;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h ; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if (color == 255)
            {
                Uint32 p = get_pixel(segmap, x, y);
                if(p == 0)
                {
                    size_t tmp = propagate(image, segmap, segval, x, y);
                    if(tmp > max)
                    {
                        max = tmp;
                        labelmax = segval;
                    }
                    segval+=1;
                }
            }   
        }
    }
    return labelmax;
}

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y)
{
    size_t n = 0;
    Uint8 color;
    Uint32 pix = get_pixel(image, x, y);
    SDL_GetRGB(pix, image->format, &color, &color, &color);
    Uint32 p = get_pixel(segmap, x, y);
    if(p != 0 || color == 0)
    {
        return 0;
    }
    put_pixel(segmap, x, y, segval);
    if(y > 0)
    {
        n += propagate(image, segmap, segval, x, y-1);
    }
    if(x > 0)
    {
        n += propagate(image, segmap, segval, x-1, y);
    }
    if(x < (size_t)image->w-1)
    {
        n += propagate(image, segmap, segval, x+1, y);
    }
    if(y < (size_t)image->h-1)
    {
        n += propagate(image, segmap, segval, x, y+1);
    }
    return 1 + n;
}

void colorimage(SDL_Surface *image, SDL_Surface* segmap)
{
    Uint8 r,g,b;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 val = get_pixel(segmap, x, y);
            r = (29*val)%256;
            g = (53*val)%256;
            b = (97*val)%256;
            put_pixel(image, x, y, SDL_MapRGB(image->format, r, g, b));
        }
    }
}

void remove_other_obj(SDL_Surface* image, SDL_Surface* segmap, size_t objval)
{
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 pix = get_pixel(segmap ,x ,y);
            if ((size_t)pix != objval)
            {
                put_pixel(image ,x ,y , SDL_MapRGB(image->format, 0, 0, 0));
            }
            else
            {
                put_pixel(image ,x ,y , SDL_MapRGB(image->format, 255, 255, 255));
            }
        }
    }
}

void find_extremity_coordinates(SDL_Surface* image, size_t* top, size_t* right, size_t* left, size_t* bottom)
{
    for (size_t y = 0; y < (size_t)image->h; y++)
    {
        for (size_t x = 0; x < (size_t)image->w; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *top = y;
            }
        }
    }
    for (size_t y = (size_t)image->h-1; y < (size_t)image->h; y--)
    {
        for (size_t x = 0; x < (size_t)image->w; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *bottom = y;
            }
        }
    }
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *left = x;
            }
        }
    }
    for (size_t x = (size_t)image->w-1; x < (size_t)image->w; x--)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *right = x;
            }
        }
    }
}