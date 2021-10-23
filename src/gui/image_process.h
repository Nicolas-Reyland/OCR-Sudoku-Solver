#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#define SAVED_IMG_NAME_G "grayscale.bmp"
#define SAVED_IMG_NAME_BL "blurred_image.bmp"
#define SAVED_IMG_NAME_BI "binarised_image.bmp"

void init_sdl();
SDL_Surface* load_image(char *path);
void grayscale(Uint8 *r, Uint8 *g, Uint8 *b);
void gaussian_blur(SDL_Surface *src_surface, int i, int j, 
	Uint8 *r_dest, Uint8 *g_dest, Uint8 *b_dest);
double sauvola_binarisation(SDL_Surface *src_surface, int i, int j);
void image_process(char *path);

#endif