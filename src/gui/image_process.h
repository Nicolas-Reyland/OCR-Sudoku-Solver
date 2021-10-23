#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#define SAVED_IMG_NAME_R "rotated_image.bmp"
#define SAVED_IMG_NAME_G "grayscale.bmp"
#define SAVED_IMG_NAME_BL "blurred_image.bmp"
#define SAVED_IMG_NAME_BI "binarised_image.bmp"

typedef enum { MEAN, 
			STD_DEVIATION } II_TYPE;

void init_sdl();

SDL_Surface* load_image(char *path);

void color_surface(SDL_Surface *surface, Uint32 pixel);

unsigned long long int **create_integral_image(SDL_Surface *surface, 
					unsigned int margin, II_TYPE ii_type);

void free_integral_image(unsigned long long int **integral_image, size_t width);

unsigned long long int integral_image_value(II_TYPE ii_type, Uint8 composant);

void grayscale(Uint8 *r, Uint8 *g, Uint8 *b);

void gaussian_blur(SDL_Surface *src_surface, int i, int j, 
	Uint8 *r_dest, Uint8 *g_dest, Uint8 *b_dest);

double sauvola_binarisation(unsigned long long int **mean_ii, 
							unsigned long long int **std_deviation_ii, 
							int i, int j);

void image_process(char *path);

void rotate_image(char *path, double angle);

#endif