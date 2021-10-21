#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap);

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y);

void colorimage(SDL_Surface* image, SDL_Surface* segmap);

void remove_other_obj(SDL_Surface* image, SDL_Surface* baseimage, SDL_Surface* segmap, size_t objval);

void find_extremity_coordinates(SDL_Surface* image, size_t* up, size_t* right, size_t* left, size_t* down);

void save_cases(SDL_Surface* image);

SDL_Surface* cut_image(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right);



void detect_grid(SDL_Surface* image, SDL_Surface* baseimage)
{
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    size_t biggestObj = segmentation(image, segmap);
    colorimage(image, segmap);
    if(SDL_SaveBMP(image, "colorised.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    remove_other_obj(image, baseimage,segmap, biggestObj);
    size_t top, right, left, bottom;
    find_extremity_coordinates(image, &top, &right, &left, &bottom);
    Uint32 pix = SDL_MapRGB(image->format, 255, 0, 0);
    put_pixel(image, right, top, pix);
    put_pixel(image, left, top, pix);
    put_pixel(image, right, bottom, pix);
    put_pixel(image, left, bottom, pix);
    baseimage = cut_image(baseimage, top, bottom, left, right);
    save_cases(baseimage);
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

void remove_other_obj(SDL_Surface* image, SDL_Surface* baseimage, SDL_Surface* segmap, size_t objval)
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
                put_pixel(baseimage ,x ,y , SDL_MapRGB(baseimage->format, 0, 0, 0));
            }
        }
    }
}

void find_extremity_coordinates(SDL_Surface* image, size_t* top, size_t* right, size_t* left, size_t* bottom)
{
    int b = 1;
    for (size_t y = 0; y < (size_t)image->h && b; y++)
    {
        for (size_t x = 0; x < (size_t)image->w && b; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *top = y;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t y = (size_t)image->h-1; y < (size_t)image->h  && b; y--)
    {
        for (size_t x = 0; x < (size_t)image->w  && b; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *bottom = y;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t x = 0; x < (size_t)image->w  && b; x++)
    {
        for (size_t y = 0; y < (size_t)image->h  && b; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *left = x;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t x = (size_t)image->w-1; x < (size_t)image->w  && b; x--)
    {
        for (size_t y = 0; y < (size_t)image->h  && b; y++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                *right = x;
                b = 0;
            }
        }
    }
}

SDL_Surface* cut_image(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right)
{
    SDL_Surface *cas = SDL_CreateRGBSurface(0, right-left+1, bottom-top+1, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    for (size_t y = top; y <= bottom; y++)
    {
        for (size_t x = left; x <= right; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            put_pixel(cas, x-left, y-top, SDL_MapRGB(cas->format, color, color, color));
        }
    }
    return cas;
}

void save_cases(SDL_Surface* image)
{
    size_t w = (size_t)image->w;
    size_t h = (size_t)image->h;
    for (size_t x = 0; x < 9; x++)
    {
        for (size_t y = 0; y < 9; y++)
        {
            SDL_Surface* cas = cut_image(image, y*h/9, (size_t)y*h/9+h/9, x*w/9, (size_t)x*w/9+w/9);
            char name[255];
            sprintf(name,"cases/case_%lu_%lu",x,y);
            if(SDL_SaveBMP(cas, name) != 0)
            {
                printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
            }
        }
    }
}