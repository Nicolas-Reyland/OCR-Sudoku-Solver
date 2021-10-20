#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operation.h"

const double pi = 3.1415926;

void Hough_transform(SDL_Surface* image, SDL_Surface* Houghspace, double AngleInterval, 
                    int NumberofAngles, double DistanceInterval, int NumberofDistances);

void Find_Lines(SDL_Surface* image, SDL_Surface* Houghspace, double AngleInterval, 
                double DistanceInterval, int diagLen);



void LineDetection(SDL_Surface* image)
{
    double diagLen = sqrt((double)(image->h*image->h+image->w*image->w));
    double maxAngle = 90;
    double AngleInterval = 1;
	double DistanceInterval =1;
    
    int NumberofAngles = (int)(maxAngle/AngleInterval)+1;
	int NumberofDistances = (int)diagLen/DistanceInterval+1;

    SDL_Surface* HoughSpace = SDL_CreateRGBSurface(0, NumberofAngles, NumberofDistances, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);

    Hough_transform(image, HoughSpace, AngleInterval, NumberofAngles, DistanceInterval, NumberofDistances);

    if(SDL_SaveBMP(HoughSpace, "HoughSpace.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    Find_Lines(image, HoughSpace, AngleInterval, DistanceInterval, (int) diagLen);
}



void Hough_transform(SDL_Surface* image, SDL_Surface* Houghspace, double AngleInterval, 
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
                    double angle = AngleInterval*((double)i);
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

void Find_Lines(SDL_Surface* image, SDL_Surface* Houghspace, double AngleInterval, 
                double DistanceInterval, int diagLen)
{
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(image);
    for (unsigned int j = 0; j < (unsigned int)Houghspace->h; j++)
    {
        for (unsigned int i = 0; i < (unsigned int)Houghspace->w; i++)
        {
            Uint32 pixel = get_pixel(image, i, j);
            if ((int)pixel > diagLen*2)
            {
                double angle = AngleInterval*((double)i);
                double distance = j*DistanceInterval;
                double a = cos(angle/180*pi);
                double b = sin(angle/180*pi);
                double x = a*distance+(image->w)/2;
                double y = b*distance+(image->h)/2;
                int x1 = (int)(x + 1000 * (-b));
                int y1 = (int)(y + 1000 * (a));
                int x2 = (int)(x - 1000 * (-b));
                int y2 = (int)(y - 1000 * (a));
                printf("%d, %d, %d, %d \n", x1, y1, x2, y2);
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }
}