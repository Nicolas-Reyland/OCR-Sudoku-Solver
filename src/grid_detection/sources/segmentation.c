#include "../headers/segmentation.h"

CCTuple* twopassSegmentation(SDL_Surface *image, SDL_Surface* segmap, size_t *segval)
{
    linked_list *linked = init_linked_list();
    set* s = init_set(NULL, 0);
    linked->append_value(linked, s);
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
                    s = init_set(NULL, *segval);
                    linked->append_value(linked, s);
                    put_pixel(segmap, x, y, *segval);
                    *segval+=1;
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

    CCTuple *histo = malloc(sizeof(CCTuple)*(*segval));
    for (size_t i = 0; i < *segval; i++)
    {
        CCTuple t = {.nb_pixels = 0, .label = i};
        histo[i] = t;
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
                histo[s->value].nb_pixels += 1;
                put_pixel(segmap, x, y, s->value);
            }
        }
    }

    printf("STARTED\n");
    heapSort(histo, *segval);
    printf("FINISHED\n");

    for (int i = 0; i < linked->length; i++)
    {
        free(linked->get_value_at(linked, i));
    }

    free_linked_list(linked);
    return histo;
}
