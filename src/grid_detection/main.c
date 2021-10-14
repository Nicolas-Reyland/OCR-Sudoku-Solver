#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "read_img.h"

int main(void)
{
    const char* path = "imgs/grayscale(0).bmp";
    read_img(path);
    return 0;
}