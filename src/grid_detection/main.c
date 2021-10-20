#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "detect_grid.h"

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface* image = IMG_Load("imgs/grid.bmp");	
    if (!image) 
    {
        printf("Failed to load image at %s: %s\n", "imgs/grid.bmp", SDL_GetError());
        return 1;
    }
    detect_grid(image);
    if(SDL_SaveBMP(image, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    return 0;
}