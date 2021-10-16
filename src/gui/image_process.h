#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#define SAVED_IMG_NAME_R "rotated_image.bmp"
#define SAVED_IMG_NAME_G "grayscale.bmp"
#define SAVED_IMG_NAME_BL "blurred_image.bmp"
#define SAVED_IMG_NAME_BI "binarised_image.bmp"

void init_sdl();
SDL_Surface* load_image(char *path);
void color_surface(SDL_Surface *surface, Uint32 pixel);
void grayscale(Uint8 *r, Uint8 *g, Uint8 *b);
void gaussian_blur(SDL_Surface *src_surface, int i, int j, 
	Uint8 *r_dest, Uint8 *g_dest, Uint8 *b_dest);
float sauvola_binarisation(SDL_Surface *src_surface, int i, int j);
void image_process(char *path);
void rotate_image(char *path, double angle);

#endif