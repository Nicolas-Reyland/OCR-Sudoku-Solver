#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

void lineDetection(SDL_Surface* image, SDL_Surface* edgemap)
{
    int edgemap_h = edgemap->h;
    int edgemap_w = edgemap->w;


    int numrho = 180;
    int numtheta = 180;

    double d = sqrt(edgemap_h*edgemap_h+edgemap_w*edgemap_w);
    double dtheta = 180 / numtheta;
    double drho = (2 * d) / numrho;

    double thetas[numtheta];
    double rhos[numrho];

    
}