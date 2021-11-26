#include "../headers/detect_grid.h"

void detect_grid(const char* file)
{
    SDL_Surface* image = IMG_Load(file);
    SDL_Surface *segmap = SDL_CreateRGBSurface(0, image->w, image->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0);
    
    size_t segval = 1;

    size_t top, right, left, bottom;
    size_t topleftX, topleftY, toprightX, toprightY, bottomleftX, bottomleftY, bottomrightX, bottomrightY;

    SDL_Surface *croppedimage, *croppedsegmap;

    size_t **coords = {
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

    CCTuple *histo = twopassSegmentation(image, segmap, &segval);

    size_t gridval = findgrid(histo, coords, surf);
    
    free(histo);

    remove_grid(image, segmap, gridval, 1);
    
    SDL_Renderer *renderer = SDL_CreateSoftwareRenderer(croppedimage);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer, topleftX, topleftY, toprightX, toprightY);
    SDL_RenderDrawLine(renderer, topleftX, topleftY, bottomleftX, bottomleftY);
    SDL_RenderDrawLine(renderer, toprightX, toprightY, bottomrightX, bottomrightY);
    SDL_RenderDrawLine(renderer, bottomleftX, bottomleftY, bottomrightX, bottomrightY);
    
    SDL_DestroyRenderer(renderer);

    save_cells(croppedimage);
    
    if(SDL_SaveBMP(croppedimage, "result.bmp") != 0)
    {
        printf("SDL_SaveBMP failed: %s\n", SDL_GetError());
    }
    
    SDL_FreeSurface(croppedimage);
    SDL_FreeSurface(croppedsegmap);
    SDL_FreeSurface(segmap);
    SDL_FreeSurface(image);
}

size_t findgrid(CCTuple *histo, size_t** coords, SDL_Surface** surf[])
{
    do
    {
        find_extremity_coordinates(segmap, gridval, &top, &right, &left, &bottom);
    
        croppedimage = cut_image(image, top, bottom, left, right);
        croppedsegmap = cut_segmap(segmap, top, bottom, left, right);

        find_corner_coordinates(croppedsegmap, gridval, &topleftX, &topleftY, &toprightX, &toprightY, &bottomleftX, &bottomleftY, &bottomrightX, &bottomrightY);
    } while ();
    
}