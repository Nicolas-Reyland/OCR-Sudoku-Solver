#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "detect_grid.h"

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    const char* file = "imgs/binarised_image(4).bmp";
	SDL_Surface* image = IMG_Load(file);	
    if (!image) 
    {
        printf("Failed to load image at %s: %s\n", file, SDL_GetError());
        return 1;
    }
    SDL_Surface* baseimage = IMG_Load(file);	
    if (!image) 
    {
        printf("Failed to load image at %s: %s\n", file, SDL_GetError());
        return 1;
    }
    detect_grid(image, baseimage);
    if(SDL_SaveBMP(image, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    return 0;
}