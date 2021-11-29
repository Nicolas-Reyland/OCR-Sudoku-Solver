#include "../headers/detect_grid.h"

void detect_grid(const char* file)
{
    SDL_Surface *image = IMG_Load(file);
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0);

    size_t top, right, left, bottom;
    size_t topleftX, topleftY, toprightX, toprightY, bottomleftX, bottomleftY, bottomrightX, bottomrightY;

    SDL_Surface *croppedimage = NULL, 
                *croppedsegmap = NULL;

    size_t *coords[] = {
        &top,
        &right,
        &left, 
        &bottom, 
        &topleftX, 
        &topleftY, 
        &toprightX, 
        &toprightY, 
        &bottomleftX, 
        &bottomleftY, 
        &bottomrightX, 
        &bottomrightY
    };

    SDL_Surface **surf[] = {
        &image,
        &segmap,
        &croppedimage,
        &croppedsegmap  
    };

    size_t histo_size = 1;
    CCTuple *histo = twopassSegmentation(image, segmap, &histo_size);

    size_t gridval = findgrid(histo, histo_size, coords, surf);
    
    free(histo);

    remove_grid(croppedimage, croppedsegmap, gridval, 1);
    
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(croppedimage);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, topleftX, topleftY, toprightX, toprightY);
    SDL_RenderDrawLine(renderer, topleftX, topleftY, bottomleftX, bottomleftY);
    SDL_RenderDrawLine(renderer, toprightX, toprightY, bottomrightX, bottomrightY);
    SDL_RenderDrawLine(renderer, bottomleftX, bottomleftY, bottomrightX, bottomrightY);
    
    SDL_DestroyRenderer(renderer);

    double size = fmax(croppedimage->h, croppedimage->w)+10;  

    double *val = malloc(sizeof(double)*8);

    val[0] = (double)topleftX;
    val[1] = (double)topleftY;
    val[2] = (double)toprightX;
    val[3] = (double)toprightY;
    val[4] = (double)bottomleftX;
    val[5] = (double)bottomleftY;
    val[6] = (double)bottomrightX;
    val[7] = (double)bottomrightY;

    Matrix *transform = findhomography(val, size);

    if(SDL_SaveBMP(croppedimage, "before.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }

    SDL_Surface *transformedimage = transformimage(transform, croppedimage, size);

    free(val);
    free_matrix(transform);

    save_cells(transformedimage);
    
    if(SDL_SaveBMP(transformedimage, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    
    SDL_FreeSurface(transformedimage);
    SDL_FreeSurface(croppedimage);
    SDL_FreeSurface(croppedsegmap);
    SDL_FreeSurface(segmap);
    SDL_FreeSurface(image);
}

size_t findgrid(CCTuple *histo, size_t histo_size, size_t* coords[], SDL_Surface** surf[])
{
    size_t  *top = coords[0],
            *right = coords[1],
            *left = coords[2], 
            *bottom = coords[3], 
            *topleftX = coords[4], 
            *topleftY = coords[5], 
            *toprightX = coords[6], 
            *toprightY = coords[7], 
            *bottomleftX = coords[8], 
            *bottomleftY = coords[9], 
            *bottomrightX = coords[10], 
            *bottomrightY = coords[11];

    SDL_Surface **image = surf[0],
                **segmap = surf[1],
                **croppedimage = surf[2],
                **croppedsegmap = surf[3];

    size_t i = histo_size - 1;
    int condition = 0;
    
    do
    {
        SDL_FreeSurface(*croppedimage);
        SDL_FreeSurface(*croppedsegmap);

        size_t current_label = histo[i].label;

        // Get coordinates
        find_extremity_coordinates(*segmap, current_label, top, right, left, bottom);
        *croppedimage = cut_image(*image, *top, *bottom, *left, *right);
        *croppedsegmap = cut_segmap(*segmap, *top, *bottom, *left, *right);
        find_corner_coordinates(*croppedsegmap, current_label, topleftX, topleftY, toprightX, toprightY, bottomleftX, bottomleftY, bottomrightX, bottomrightY);
        
        // Get the four distances
        int d_top = get_distance((int)*topleftX, (int)*topleftY, (int)*toprightX, (int)*toprightY);
        int d_right = get_distance((int)*toprightX, (int)*toprightY, (int)*bottomrightX, (int)*bottomrightY);
        int d_bottom = get_distance((int)*bottomrightX, (int)*bottomrightY, (int)*bottomleftX, (int)*bottomleftY);
        int d_left = get_distance((int)*bottomleftX, (int)*bottomleftY, (int)*topleftX, (int)*topleftY);

        // Check if component is a potential grid
        int error_margin = 2*d_top / 9;

        if (!((d_right < (d_top - error_margin) || d_right > (d_top + error_margin)) ||
            (d_bottom < (d_top - error_margin) || d_bottom > (d_top + error_margin)) ||
            (d_left < (d_top - error_margin) || d_left > (d_top + error_margin))))
        {
            condition = 1;
        }
        i -= 1;

    } while (i > 0 && condition == 0 && histo[i].nb_pixels > (size_t)(*image)->h*(*image)->w/250);

    if(condition == 0 && i == 0)
    {
        errx(1, "Error: no grid is found in the given image.");
    }
    
    printf("LABEL -> %zu\n", histo[i+1].label);
    printf("NB PIXELS -> %zu\n", histo[i+1].nb_pixels);

    return histo[i+1].label;
}

int get_distance(int X1, int Y1, int X2, int Y2)
{
    int d1 = abs(X1 - X2);
    int d2 = abs(Y1 - Y2);

    int distance = (d1 * d1) + (d2 * d2);
    distance = (int)round(sqrt((double)distance));

    return distance;
}