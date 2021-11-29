#include "../headers/grid_utils.h"

void colorizeimage(SDL_Surface *image, SDL_Surface* segmap)
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


void remove_grid(SDL_Surface* image, SDL_Surface* segmap, size_t objval, int b)
{
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint32 pix = get_pixel(segmap ,x ,y);
            if ((size_t)pix == objval)
            {
                if (b)
                {
                    put_pixel(image ,x ,y , SDL_MapRGB(image->format, 0, 0, 0));
                }
            }
            else
            {
                if (!b)
                {
                    put_pixel(image ,x ,y , SDL_MapRGB(image->format, 0, 0, 0));
                }
            }
        }
    }
}


void find_extremity_coordinates(SDL_Surface* segmap, size_t objval, size_t* top, size_t* right, size_t* left, size_t* bottom)
{
    int b = 1;
    Uint32 pix;
    for (size_t y = 0; y < (size_t)segmap->h && b; y++)
    {
        for (size_t x = 0; x < (size_t)segmap->w && b; x++)
        {
            pix = get_pixel(segmap, x, y);
            if(pix == objval)
            {
                *top = y;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t y = (size_t)segmap->h-1; y < (size_t)segmap->h  && b; y--)
    {
        for (size_t x = 0; x < (size_t)segmap->w  && b; x++)
        {
            pix = get_pixel(segmap, x, y);
            if(pix == objval)
            {
                *bottom = y;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t x = 0; x < (size_t)segmap->w  && b; x++)
    {
        for (size_t y = 0; y < (size_t)segmap->h  && b; y++)
        {
            pix = get_pixel(segmap, x, y);
            if(pix == objval)
            {
                *left = x;
                b = 0;
            }
        }
    }
    b = 1;
    for (size_t x = (size_t)segmap->w-1; x < (size_t)segmap->w  && b; x--)
    {
        for (size_t y = 0; y < (size_t)segmap->h  && b; y++)
        {
            pix = get_pixel(segmap, x, y);
            if(pix == objval)
            {
                *right = x;
                b = 0;
            }
        }
    }
}


void find_corner_coordinates(SDL_Surface* segmap, size_t objval, size_t* topleftX, size_t* topleftY, size_t* toprightX, size_t* toprightY, 
                                size_t* bottomleftX, size_t* bottomleftY, size_t* bottomrightX, size_t* bottomrightY)
{
    size_t min = segmap->h+segmap->w;
    Uint32 pix;

    for (size_t x = 0; x < (size_t)segmap->w/2; x++)
    {
        for (size_t y = 0; y < (size_t)segmap->h/2; y++)
        {
            pix = get_pixel(segmap, x, y);
            if (pix == objval)
            {
                if (x+y < min)
                {
                    min = x+y;
                    *topleftX = x;
                    *topleftY = y;
                }
                break;
            }   
        }
    }
    
    min = segmap->h+segmap->w;
    
    for (size_t x = 0; x < (size_t)segmap->w/2; x++)
    {
        for (size_t y = 0; y < (size_t)segmap->h/2; y++)
        {
            pix = get_pixel(segmap, x, segmap->h-1-y);
            if (pix == objval)
            {
                if (x+y < min)
                {
                    min = x+y;
                    *bottomleftX = x;
                    *bottomleftY = y;
                }
                break;
            }   
        }
    }

    *bottomleftY = segmap->h-1-*bottomleftY;
    
    min = segmap->h+segmap->w;
    
    for (size_t x = 0; x < (size_t)segmap->w/2; x++)
    {
        for (size_t y = 0; y < (size_t)segmap->h/2; y++)
        {
            pix = get_pixel(segmap, segmap->w-1-x, y);
            if (pix == objval)
            {
                if (x+y < min)
                {
                    min = x+y;
                    *toprightX = x;
                    *toprightY = y;
                }
                break;
            }   
        }
    }

    *toprightX = segmap->w-1-*toprightX;
    
    min = segmap->h+segmap->w;
    
    for (size_t x = 0; x < (size_t)segmap->w/2; x++)
    {
        for (size_t y = 0; y < (size_t)segmap->h/2; y++)
        {
            pix = get_pixel(segmap, segmap->w-1-x, segmap->h-1-y);
            if (pix == objval)
            {
                if (x+y < min)
                {
                    min = x+y;
                    *bottomrightX = x;
                    *bottomrightY = y;
                }
            }   
        }
    }

    *bottomrightX = segmap->w-1-*bottomrightX;
    *bottomrightY = segmap->h-1-*bottomrightY;
}

SDL_Surface* cut_segmap(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right)
{
    SDL_Surface *newsegmap = SDL_CreateRGBSurface(0, right-left+1, bottom-top+1, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
    for (size_t y = top; y <= bottom; y++)
    {
        for (size_t x = left; x <= right; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            put_pixel(newsegmap, x-left, y-top, pix);
        }
    }
    return newsegmap;
}

SDL_Surface* cut_image(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right)
{
    SDL_Surface *cell = SDL_CreateRGBSurface(0, right-left+1, bottom-top+1, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
    for (size_t y = top; y <= bottom; y++)
    {
        for (size_t x = left; x <= right; x++)
        {
            Uint32 pix = get_pixel(image, x, y);
            Uint8 color;
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            put_pixel(cell, x-left, y-top, SDL_MapRGB(cell->format, color, color, color));
        }
    }
    return cell;
}


void save_cells(SDL_Surface* image)
{

    size_t w = (size_t)image->w;
    size_t h = (size_t)image->h;
    for (size_t x = 0; x < 9; x++)
    {
        for (size_t y = 0; y < 9; y++)
        {
            SDL_Surface* cell = cut_image(image, y*h/9, (size_t)y*h/9+h/9, x*w/9, (size_t)x*w/9+w/9);
            SDL_Surface* cellsegmap = SDL_CreateRGBSurface(0, w/9+1, h/9+1, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
            
            size_t limit = (h/9*w/9)/60;

            size_t X = 0;
            size_t size = 0;
            do
            {
                size = propagate(cell, cellsegmap, 1, w/18+X, h/18);
                size_t size2 = propagate(cell, cellsegmap, 1, w/18-X, h/18);
                if (size2 > size)
                {
                    size = size2;
                }
                put_pixel(image, x*h/9+h/18+X, y*h/9+h/18, SDL_MapRGB(image->format, 255, 0, 0));
                put_pixel(image, x*h/9+h/18-X, y*h/9+h/18, SDL_MapRGB(image->format, 255, 0, 0));
                X++;
            } while (size < limit && X < w/36);

            if (size > limit)
            {
                SDL_Surface* emptycell = SDL_CreateRGBSurface(0, 28, 28, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0);
                size_t top, right, left, bottom;
                find_extremity_coordinates(cellsegmap, 1, &top, &right, &left, &bottom);
                SDL_Surface* croppedcell = cut_image(cell, top, bottom, left, right);

                int max = (int) ceil((double)(croppedcell->w)/(double)22);
                if (((int) ceil((double)(croppedcell->h)/(double)22)) > max)
                {
                    max = (int) ceil((double)(croppedcell->h)/(double)24);
                }

                SDL_Surface* shrinkedcroppedcell = shrinkSurface(croppedcell, max, max);

                copy_symbol(emptycell, shrinkedcroppedcell, (28-shrinkedcroppedcell->w)/2, (28-shrinkedcroppedcell->h)/2);
                SDL_FreeSurface(croppedcell);
                SDL_FreeSurface(shrinkedcroppedcell);

                char name[255];
                sprintf(name,"cells/cell_%lu_%lu",x,y);
                if(SDL_SaveBMP(emptycell, name) != 0)
                {
                    printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
                }
                SDL_FreeSurface(emptycell);
            }
            SDL_FreeSurface(cell);
            SDL_FreeSurface(cellsegmap);
        }
    }
}

void copy_symbol(SDL_Surface* output, SDL_Surface* input, size_t left, size_t top)
{
    for (size_t x = 0; x < (size_t)input->w; x++)
    {
        for (size_t y = 0; y < (size_t)input->h; y++)
        {
            Uint32 pix = get_pixel(input, x, y);
            Uint8 color;
            SDL_GetRGB(pix, input->format, &color, &color, &color);
            put_pixel(output, x+left, y+top, SDL_MapRGB(output->format, color, color, color));   
        }
    }
    
}