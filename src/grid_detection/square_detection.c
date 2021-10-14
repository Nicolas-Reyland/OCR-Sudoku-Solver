#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

const double pi = 3.1415926;

void Hough_transform(SDL_Surface* image, SDL_Surface* Houghspace, double minAngle, double AngleInterval, 
                    int NumberofAngles, double DistanceInterval, int NumberofDistances);

SDL_Surface* LineDetection(SDL_Surface* image)
{
    double diagLen = sqrt((double)(image->h*image->h+image->w*image->w));
    double minAngle = 0;
    double maxAngle = 90;
    double AngleInterval = 1;
	double DistanceInterval =1;
    
    int NumberofAngles = (int)(maxAngle/AngleInterval)+1;
	int NumberofDistances = (int)diagLen/DistanceInterval+1;

    SDL_Surface* HoughSpace = SDL_CreateRGBSurface(0, NumberofAngles, NumberofDistances, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    Hough_transform(image, HoughSpace, minAngle, AngleInterval, NumberofAngles, DistanceInterval, NumberofDistances);

    return HoughSpace;
}



void Hough_transform(SDL_Surface* image, SDL_Surface* Houghspace, double minAngle, double AngleInterval, 
                    int NumberofAngles, double DistanceInterval, int NumberofDistances)
{
    for (unsigned int y = 0; y < (unsigned int)image->h; y++)
    {
        for (unsigned int x = 0; x < (unsigned int)image->w; x++)
        {
            Uint32 pixel = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pixel, image->format, &color, &color, &color);
            if (color != 0)
            {
                for (int i = 0; i < NumberofAngles; i++)
                {
                    double angle = minAngle+AngleInterval*((double)i);
                    double distance = ((double)x)*cos(angle/180*pi)+((double)y)*sin(angle/180*pi);
                    int j = (int)(distance/DistanceInterval);
                    if (j < NumberofDistances)
                    {
                        Uint32 pix = get_pixel(Houghspace, i, j);
                        put_pixel(Houghspace, i, j, pix+1);
                        
                    }
                }
            }
        }
    }
}