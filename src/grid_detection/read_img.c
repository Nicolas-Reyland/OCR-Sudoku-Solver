#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"
#include "square_detection.h"

void vertical_filter(SDL_Surface* image, SDL_Surface* vmask)
{
    SDL_PixelFormat* fmt = image->format;
    for (unsigned int x = 0; (int)x < image->w; x++)
    {
        for (unsigned int y = 0; (int)y < (image->h)-1; y++)
        {
            Uint32 pix1 = get_pixel(image, x, y);
            Uint32 pix2 = get_pixel(image, x, y+1);
            Uint8 color1, color2;
            SDL_GetRGB(pix1, fmt, &color1, &color1, &color1);
            SDL_GetRGB(pix2, fmt, &color2, &color2, &color2); 
            Uint8 color = abs(color2-color1); 
            put_pixel(vmask, x, y, SDL_MapRGB(vmask->format, color, color, color));
        }
    }
}

void horizontal_filter(SDL_Surface* image, SDL_Surface* hmask)
{
    SDL_PixelFormat* fmt = image->format;
    for (unsigned int y = 0; (int)y < image->h; y++)
    {
        for (unsigned int x = 0; (int)x < (image->w)-1; x++)
        {
            Uint32 pix1 = get_pixel(image, x, y);
            Uint32 pix2 = get_pixel(image, x + 1, y);
            Uint8 color1, color2;
            SDL_GetRGB(pix1, fmt, &color1, &color1, &color1);
            SDL_GetRGB(pix2, fmt, &color2, &color2, &color2);
            Uint8 color = abs(color2-color1); 
            put_pixel(hmask, x, y, SDL_MapRGB(hmask->format, color, color, color));
        }
    }
}

SDL_Surface* merge_masks(SDL_Surface* hmask, SDL_Surface* vmask)
{
    SDL_Surface* result = SDL_CreateRGBSurface(0, hmask->w, hmask->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    for (unsigned int y = 0; (int)y < result->h; y++)
    {
        for (unsigned int x = 0; (int)x < result->w; x++)
        {
            Uint32 pix1 = get_pixel(hmask, x, y);
            Uint32 pix2 = get_pixel(vmask, x, y);
            Uint8 color1, color2;
            SDL_GetRGB(pix1, hmask->format, &color1, &color1, &color1);
            SDL_GetRGB(pix2, vmask->format, &color2, &color2, &color2);
            Uint8 color = sqrt(color1*color1+color2*color2); 
            if (color <= 25)
            {
                color = 0;
            }
            else 
            {
                color = 255;
            }
            put_pixel(result, x, y, SDL_MapRGB(result->format, color, color, color));
        }
    }
    return result;
}

void read_img(const char *path)
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* image = SDL_LoadBMP_RW(SDL_RWFromFile(path, "rb"), 1);	
    if (!image) 
    {
        printf("Failed to load image at %s: %s\n", path, SDL_GetError());
        return;
    }
    SDL_Surface *hmask = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    SDL_Surface *vmask = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    horizontal_filter(image, hmask);
    vertical_filter(image, vmask);
    SDL_Surface* result = merge_masks(hmask, vmask);
    if(SDL_SaveBMP(result, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    LineDetection(result);
    if(SDL_SaveBMP(result, "resultingLines.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    LineDetection(result);
    SDL_Quit();
}