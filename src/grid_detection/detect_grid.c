#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "pixel_operations.h"
#include "../utils/mem/linked_list.h"

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap);

void rowssegmentation(SDL_Surface *image, SDL_Surface* segmap);

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y);

void colorimage(SDL_Surface* image, SDL_Surface* segmap);

void remove_other_obj(SDL_Surface* image, SDL_Surface* baseimage, SDL_Surface* segmap, size_t objval);

void find_extremity_coordinates(SDL_Surface* image, size_t* up, size_t* right, size_t* left, size_t* down);

SDL_Surface* cut_image(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right);

void save_cases(SDL_Surface* image);

SDL_Surface* reduce_res(SDL_Surface* image);


void detect_grid(SDL_Surface* image, SDL_Surface* baseimage)
{
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    size_t biggestObj = segmentation(image, segmap);
    colorimage(image, segmap);
    if(SDL_SaveBMP(image, "colorised.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    remove_other_obj(image, baseimage, segmap, biggestObj);
    if(SDL_SaveBMP(image, "isolated_grid.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    size_t top, right, left, bottom;
    find_extremity_coordinates(image, &top, &right, &left, &bottom);
    baseimage = cut_image(baseimage, top, bottom, left, right);
    save_cases(baseimage);
    if(SDL_SaveBMP(baseimage, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
}

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap)
{
    size_t max = 0;
    int labelmax = 0;
    int segval = 1;
    for (size_t x = 0; x < (size_t)image->w; x+=2)
    {
        for (size_t y = 0; y < (size_t)image->h ; y+=2)
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
    printf("number of element = %d\n", segval);
    printf("max pixels in an element = %lu\n", max);
    return labelmax;
}

/*

void rowssegmentation(SDL_Surface *image, SDL_Surface* segmap)
{
    linked_list *linked = init_linked_list();
    size_t segval = 1;
    Uint32 p;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint8 color;
            Uint32 pix = get_pixel(image, x, y);
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            if(color == 255)
            {
                linked_list *neighbors = init_linked_list();
                if(x > 0)
                {
                    p = get_pixel(segmap, x-1, y);
                    if(p!=0)
                    {
                        neighbors->append_value(neighbors, p);
                    }
                }
                if(y > 0)
                {
                    p = get_pixel(segmap, x, y-1);
                    if(p!=0)
                    {
                        neighbors->append_value(neighbors, p);
                    }
                }
                if(neighbors->length == 0)
                {
                    linked_list *l = init_linked_list();
                    linked->append_value(linked, l);
                    put_pixel(segmap, x, y, segval);
                    segval+=1;
                }
                else
                {
                    p = neighbors->get_value_at(neighbors, 0);
                    for (size_t i = 0; i < neighbors->length; i++)
                    {
                        Uint32 p1 = neighbors->get_value_at(neighbors, 0);
                        if(p1 < p)
                        {
                            p = p1;
                        }
                    }
                    put_pixel(segmap, x, y, p);
                    for (size_t i = 0; i < neighbors->length; i++)
                    {
                        unite(linked->get_value_at(linked, i), neighbors);
                        linked->insert_value_at(linked, neighbors->get_value_at(neighbors, i), 0);
                    }
                    
                }
            }
        }
    } 
    for (size_t x = 0; x < image->w; x++)
    {
        for (size_t y = 0; y < image->h; y++)
        {
            p = get_pixel(segmap, x, y)
            if(p != 0)
            {
                put_pixel(segmap, x, y, );
            }
        }
        
    }
      
}


void unite(linked_list *l1, linked_list *l2)
{
    for (size_t i = 0; i < l2->lenght-1; i++)
    {
        if(l1->index_of(l1, l2->get_value_at(l2, i)) == -1)
        {
            l1->append_value(l1, l2->get_value_at(l2, i));
        }
    }
}
*/

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y)
{
    size_t n = 0;
    if(x < (size_t)image->w && y < (size_t)image->h)
    {
        Uint8 color;
        Uint32 pix = get_pixel(image, x, y);
        SDL_GetRGB(pix, image->format, &color, &color, &color);
        Uint32 p = get_pixel(segmap, x, y);
        if(p != 0 || color != 255)
        {
            return 0;
        }
        put_pixel(segmap, x, y, segval);
        size_t x1 = x;
        while (x1 < (size_t)image->w-1)
        {
            x1+=1;
            Uint32 pix = get_pixel(image, x1, y);
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            Uint32 p = get_pixel(segmap, x1, y);
            if(p == 0 && color == 255)
            {
                put_pixel(segmap, x1, y, segval);
                n+=1;
                n += propagate(image, segmap, segval, x1, y-1);
                n += propagate(image, segmap, segval, x1, y+1);
            }
            else
            {
                break;
            }
        }
        x1=x;
        while (x1 > 0)
        {
            x1-=1;
            Uint32 pix = get_pixel(image, x1, y);
            SDL_GetRGB(pix, image->format, &color, &color, &color);
            Uint32 p = get_pixel(segmap, x1, y);
            if(p == 0 && color == 255)
            {
                put_pixel(segmap, x1, y, segval);
                n+=1;
                n += propagate(image, segmap, segval, x1, y-1);
                n += propagate(image, segmap, segval, x1, y+1);
            }
            else
            {
                break;
            }
        }
        return 1 + n;
    }
    return 0;
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
                put_pixel(baseimage , x, y, SDL_MapRGB(baseimage->format, 0, 0, 0));
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

SDL_Surface* reduce_res(SDL_Surface* image)
{
    SDL_Surface *new = SDL_CreateRGBSurface(0, image->w/2, image->h/2, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
    for (size_t x = 0; x < (size_t)image->w/2; x++)
    {
        for (size_t y = 0; y < (size_t)image->h/2; y++)
        {
            Uint32 p1,p2,p3,p4;
            p1 = get_pixel(image, 2*x, 2*y);
            p2 = get_pixel(image, 2*x+1, 2*y);
            p3 = get_pixel(image, 2*x, 2*y+1);
            p4 = get_pixel(image, 2*x+1, 2*y+1);
            Uint8 c,c1,c2,c3,c4;
            SDL_GetRGB(p1, image->format, &c1, &c1, &c1);
            SDL_GetRGB(p2, image->format, &c2, &c2, &c2);
            SDL_GetRGB(p3, image->format, &c3, &c3, &c3);
            SDL_GetRGB(p4, image->format, &c4, &c4, &c4);
            c = (c1 + c2 + c3 + c4)/4;
            if (c > 128)
            {
                c = 255;
            }
            else
            {
                c = 0;
            }
            put_pixel(new, x, y, SDL_MapRGB(new->format, c, c, c));
        }
    }
    return new;
}