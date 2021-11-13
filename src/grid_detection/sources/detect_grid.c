#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include "../headers/pixel_operations.h"
#include "../../utils/mem/linked_list.h"
#include "../headers/set.h"

size_t segmentation(SDL_Surface* image, SDL_Surface* segmap);

size_t twopassSegmentation(SDL_Surface *image, SDL_Surface* segmap);

size_t propagate(SDL_Surface *image, SDL_Surface* segmap, int segval, size_t x, size_t y);

void colorimage(SDL_Surface* image, SDL_Surface* segmap);

void remove_other_obj(SDL_Surface* image, SDL_Surface* baseimage, SDL_Surface* segmap, size_t objval);

void find_extremity_coordinates(SDL_Surface* image, size_t* up, size_t* right, size_t* left, size_t* down);

SDL_Surface* cut_image(SDL_Surface* image, size_t top, size_t bottom, size_t left, size_t right);

void save_cells(SDL_Surface* image);

SDL_Surface* reduce_res(SDL_Surface* image);


void detect_grid(SDL_Surface* image, SDL_Surface* baseimage)
{
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    size_t biggestObj = twopassSegmentation(image, segmap);
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
    SDL_FreeSurface(segmap);
    size_t top, right, left, bottom;
    find_extremity_coordinates(image, &top, &right, &left, &bottom);
    SDL_Surface *croppedbaseimage = cut_image(baseimage, top, bottom, left, right);
    save_cells(croppedbaseimage);
    if(SDL_SaveBMP(croppedbaseimage, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(croppedbaseimage);
}

size_t twopassSegmentation(SDL_Surface *image, SDL_Surface* segmap)
{
    linked_list *linked = init_linked_list();
    set* s = init_set(NULL, 0);
    linked->append_value(linked, s);
    size_t segval = 1;
    Uint32 p;
    Uint32 p1,p2;
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            Uint8 color;
            p = get_pixel(image, x, y);
            SDL_GetRGB(p, image->format, &color, &color, &color);
            size_t length = 0;
            if(color == 255)
            {
                int b[2] = {0,0};
                if(x > 0)
                {
                    p1 = get_pixel(segmap, x-1, y);
                    if(p1!=0)
                    {
                        b[0] = 1;
                        length+=1;
                    }
                }
                if(y > 0)
                {
                    p2 = get_pixel(segmap, x, y-1);
                    if(p2!=0)
                    {
                        b[1] = 1;
                        length+=1;
                    }
                }
                if(length == 0)
                {
                    s = init_set(NULL, segval);
                    linked->append_value(linked, s);
                    put_pixel(segmap, x, y, segval);
                    segval+=1;
                }
                else if (length == 1)
                {
                    if (b[0])
                    {
                        put_pixel(segmap, x, y, (size_t)p1);
                    }
                    else
                    {
                        put_pixel(segmap, x, y, (size_t)p2);
                    }
                }
                else
                {
                    p = p1;
                    if (p1 != p2)
                    {
                        if (p2 < p1)
                        {
                            p = p2;
                        }
                        set* s1 = linked->get_value_at(linked, (int)p1);
                        set* s2 = linked->get_value_at(linked, (int)p2);
                        s1 = get_root(s1);
                        s2 = get_root(s2);
                        if (s1->value != s2->value)
                        {
                            if (s1->value < s2->value)
                            {
                                s2->parent = s1;
                            }                                
                            else
                            {
                                s1->parent = s2;
                            }
                        }
                    }
                    put_pixel(segmap, x, y, p);
                }
            }
        }
    }
    size_t histo[segval];
    for (size_t i = 0; i < segval; i++)
    {
        histo[i] = 0;   
    }
    for (size_t x = 0; x < (size_t)image->w; x++)
    {
        for (size_t y = 0; y < (size_t)image->h; y++)
        {
            p = get_pixel(segmap, x, y);
            if(p != 0)
            {
                s = linked->get_value_at(linked, p);
                s = get_root(s);
                histo[s->value] += 1;
                put_pixel(segmap, x, y, s->value);
            }
        }
    }
    size_t maxindex = 0;
    for (size_t i = 0; i < segval; i++)
    {
        if (histo[maxindex] < histo[i])
        {
            maxindex = i;
        }
    }
    for (int i = 0; i < linked->length; i++)
    {
        free(linked->get_value_at(linked, i));
    }
    free_linked_list(linked);
    printf("number of pixels in biggest object = %lu\n", histo[maxindex]);
    return maxindex;
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
    Uint8 color;
    Uint32 pix;
    for (size_t y = 0; y < (size_t)image->h && b; y++)
    {
        for (size_t x = 0; x < (size_t)image->w && b; x++)
        {
            pix = get_pixel(image, x, y);
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
            pix = get_pixel(image, x, y);
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
            pix = get_pixel(image, x, y);
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
            pix = get_pixel(image, x, y);
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
    SDL_Surface *cell = SDL_CreateRGBSurface(0, right-left+1, bottom-top+1, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
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
            char name[255];
            sprintf(name,"cells/cell_%lu_%lu",x,y);
            if(SDL_SaveBMP(cell, name) != 0)
            {
                printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
            }
            SDL_FreeSurface(cell);
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