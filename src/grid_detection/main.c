#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "detect_grid.h"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("Error when executing main\n");
        printf("Usage: ./main <image location>\n");
        return 1;
    }
    SDL_Init(SDL_INIT_VIDEO);
    const char* file = argv[1];
	SDL_Surface* image = IMG_Load(file);
    SDL_Surface* baseimage = IMG_Load(file);		
    if (!image) 
    {
        printf("Failed to load image at %s: %s\n", file, SDL_GetError());
        return 1;
    }	
    detect_grid(image, baseimage);
    return 0;
}