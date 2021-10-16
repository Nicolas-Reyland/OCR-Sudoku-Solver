#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include <math.h>
#include <err.h>
#include "image_process.h"
#include "pixel_operations.h"

#define PI 3.14159265358979323846

static const int g_blur_kernel_5[5][5] = {{1, 4, 7, 4, 1},
										{4, 16, 26, 16, 4},
										{7, 26, 41, 26, 7},
										{4, 16, 26, 16, 4},
										{1, 4, 7, 4, 1}};

#define G_BLUR_KERNEL_SIZE 5
#define G_BLUR_KERNEL_SUM 273

static const int s_binarisation_mean_mat[3][3] = 
										{{1, 1, 1},
										{1, 1, 1},
										{1, 1, 1}};

#define S_BINARISATION_SIZE 3
// K takes a value from the interval [0.2, 0.5]
#define K_BINARISATION 0.02f
// R is the max value of the standard deviation of an image in grayscale
#define R 128

void init_sdl()
{
	if(SDL_Init(SDL_INIT_VIDEO != 0))
		errx(1, "Failed to initialize SDL -> %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
	SDL_Surface *image;

	image = IMG_Load(path);
	if(!image)
		errx(3, "Failed to load %s: %s", path, IMG_GetError());

	return image;
}

void color_surface(SDL_Surface *surface, Uint32 pixel)
{
	unsigned int surf_width = surface->w;
	unsigned int surf_height = surface->h;

	for(unsigned int i = 0; i < surf_width; i++)
	{
		for (unsigned int j = 0; j < surf_height;j++)
		{
			put_pixel(surface, i, j, pixel);
		}
	}
}

void grayscale(Uint8 *r, Uint8 *g, Uint8 *b)
{
	Uint8 grayscale;
	///* Mean method
	grayscale = (*r + *g + *b) / 3;
	//*/

	/* BGR2GRAY -> opencv method
	grayscale = *g * 0.7152f;
	*/

	*r = grayscale;
	*g = grayscale;
	*b = grayscale;
}

// Calculates the value of a pixel after applying a gaussian blur process
void gaussian_blur(SDL_Surface *src_surface, int i, int j, 
	Uint8 *r_dest, Uint8 *g_dest, Uint8 *b_dest)
{
	Uint32 pixel;
	Uint8 current_r;

	int src_surface_w = src_surface->w;
	int src_surface_h = src_surface->h;

	int r_sum = 0;

	int middle_coordinates = G_BLUR_KERNEL_SIZE / 2;
	int current_i = i - middle_coordinates;
	int current_j = j - middle_coordinates;

	for (int x = 0; x < G_BLUR_KERNEL_SIZE; x++)
	{
		for (int y = 0; y < G_BLUR_KERNEL_SIZE; y++)
		{
			if(current_i >= 0 && current_i < src_surface_w && 
				current_j >= 0 && current_j < src_surface_h)
			{
				pixel = get_pixel(src_surface, current_i, current_j);
				SDL_GetRGB(pixel, src_surface->format, 
					&current_r, &current_r, &current_r);

				r_sum += g_blur_kernel_5[x][y] * current_r;
			}

			current_j += 1;
		}

		current_j = j - middle_coordinates;
		current_i += 1;
	}

	*r_dest = r_sum / G_BLUR_KERNEL_SUM;
	*g_dest = *r_dest;
	*b_dest = *r_dest;
}

float sauvola_binarisation(SDL_Surface *src_surface, int i, int j)
{
	Uint32 pixel;
	Uint8 current_r;

	int s_bin_size_pow_2 = S_BINARISATION_SIZE * S_BINARISATION_SIZE;

	float mean = 0.0f;
	float std_deviation = 0.0f;
	float threshold = 0.0f;

	int src_surface_w = src_surface->w;
	int src_surface_h = src_surface->h;

	int middle_coordinates = S_BINARISATION_SIZE / 2;
	int current_i = i - middle_coordinates;
	int current_j = j - middle_coordinates;

	for (int x = 0; x < S_BINARISATION_SIZE; x++)
	{
		for (int y = 0; y < S_BINARISATION_SIZE; y++)
		{
			if(current_i >= 0 && current_i < src_surface_w && 
				current_j >= 0 && current_j < src_surface_h)
			{
				pixel = get_pixel(src_surface, current_i, current_j);
				SDL_GetRGB(pixel, src_surface->format, 
					&current_r, &current_r, &current_r);

				mean += s_binarisation_mean_mat[x][y] * current_r;
			}

			current_j += 1;
		}

		current_j = j - middle_coordinates;
		current_i += 1;
	}

	mean /= s_bin_size_pow_2;

	current_i = i - middle_coordinates;
	current_j = j - middle_coordinates;

	for (int x = 0; x < S_BINARISATION_SIZE; x++)
	{
		for (int y = 0; y < S_BINARISATION_SIZE; y++)
		{
			if(current_i >= 0 && current_i < src_surface_w && 
				current_j >= 0 && current_j < src_surface_h)
			{
				pixel = get_pixel(src_surface, current_i, current_j);
				SDL_GetRGB(pixel, src_surface->format, 
					&current_r, &current_r, &current_r);

				std_deviation += pow((current_r - mean), 2);
			}

			current_j += 1;
		}

		current_j = j - middle_coordinates;
		current_i += 1;
	}

	std_deviation /= s_bin_size_pow_2;
	std_deviation = sqrt(std_deviation);

	threshold = mean * (1 + K_BINARISATION * ((std_deviation / R) - 1));

	return threshold;
}

void image_process(char *path)
{
	SDL_Surface *surface = NULL;
	Uint32 pixel;
	Uint8 r, g, b;
	float threshold;

	init_sdl();
	surface = load_image(path);

	int surf_width = surface->w;
	int surf_height = surface->h;

	//===================================================
	//****************Grayscale process******************
	//===================================================

	for(int i = 0; i < surf_width; i++)
	{
		for (int j = 0; j < surf_height;j++)
		{
			pixel = get_pixel(surface, i, j);
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);

			grayscale(&r, &g, &b);

			pixel = SDL_MapRGB(surface->format, r, g, b);
			put_pixel(surface, i, j, pixel);
		}
	}

	SDL_SaveBMP(surface, SAVED_IMG_NAME_G);

	//===================================================
	//******************G.Blur process*******************
	//===================================================

	// Transparency is not activated
	SDL_Surface *blurred_surface = SDL_CreateRGBSurface (0, 
		surf_width, surf_height, 32, 0, 0, 0, 0);

	if(blurred_surface == NULL)
	{
	    printf("Failed to create surface -> %s\n", SDL_GetError());
	}

	for(int i = 0; i < surf_width; i++)
	{
		for (int j = 0; j < surf_height;j++)
		{
			//pixel = get_pixel(blurred_surface, i, j);
			//SDL_GetRGB(pixel, blurred_surface->format, &r, &g, &b);

			gaussian_blur(surface, i, j, &r, &g, &b);

			pixel = SDL_MapRGB(blurred_surface->format, r, g, b);
			put_pixel(blurred_surface, i, j, pixel);
		}
	}

	SDL_FreeSurface(surface);
	SDL_SaveBMP(blurred_surface, SAVED_IMG_NAME_BL);

	//===================================================
	//***********Sauvola binarisation process************
	//===================================================

	// Transparency is not activated
	SDL_Surface *binarised_surface = SDL_CreateRGBSurface (0, 
		surf_width, surf_height, 32, 0, 0, 0, 0);

	if(binarised_surface == NULL)
	{
	    printf("Failed to create surface -> %s\n", SDL_GetError());
	}

	for(int i = 0; i < surf_width; i++)
	{
		for (int j = 0; j < surf_height;j++)
		{
			pixel = get_pixel(blurred_surface, i, j);
			SDL_GetRGB(pixel, blurred_surface->format, &r, &g, &b);

			threshold = sauvola_binarisation(blurred_surface, i, j);

			//r = r >= threshold;
			//r = r * 255;
			
			if (r >= threshold)
				r = 0;
			else
				r = 255;

			pixel = SDL_MapRGB(binarised_surface->format, r, r, r);
			put_pixel(binarised_surface, i, j, pixel);
		}
	}

	SDL_SaveBMP(binarised_surface, SAVED_IMG_NAME_BI);
	SDL_FreeSurface(blurred_surface);
	SDL_FreeSurface(binarised_surface);
}

// The angle given is in degrees
void rotate_image(char *path, double angle)
{
	SDL_Surface *src_surface = NULL;
	SDL_Surface *rotated_surface = NULL;
	Uint32 pixel;
	Uint8 r, g, b;

	int x = 0;
	int y = 0;

	int rotated_x = 0;
	int rotated_y = 0;

	angle = angle * 2 * PI / 360;

	init_sdl();
	src_surface = load_image(path);

	int src_surf_w = src_surface->w;
	int src_surf_h = src_surface->h;

	int half_src_surf_w = src_surf_w / 2;
	int half_src_surf_h = src_surf_h / 2;

	int rot_surf_edge = (int)sqrt(src_surf_w * src_surf_w + 
							src_surf_h * src_surf_h) + 1;

	// Transparency is not activated
	// Creation of a white surface
	rotated_surface = SDL_CreateRGBSurface (0, 
		rot_surf_edge, rot_surf_edge, 32, 0, 0, 0, 0);

	color_surface(rotated_surface,
				SDL_MapRGB(rotated_surface->format, 255, 255, 255));

	for(int i = 0; i < src_surf_w; i++)
	{
		for (int j = 0; j < src_surf_h;j++)
		{
			pixel = get_pixel(src_surface, i, j);
			SDL_GetRGB(pixel, src_surface->format, &r, &g, &b);
			pixel = SDL_MapRGB(rotated_surface->format, r, g, b);
			
			x = i - half_src_surf_w;
			y = half_src_surf_h - j;
			
			
			//===================================================
			//*********************Rotation**********************
			//===================================================

			//rotated_x = round(x * cos(angle) - y * sin(angle));
			//rotated_y = round(x * sin(angle) + y * cos(angle));

			// Shear 1
			rotated_x = round(x-y*tan(angle/2));
			rotated_y = y;

			// Shear 2
			rotated_y = round(rotated_x*sin(angle)+rotated_y);

			// Shear 3
			rotated_x = round(rotated_x-rotated_y*tan(angle/2));

			rotated_x = rotated_x + half_src_surf_w;
			rotated_y = half_src_surf_h - rotated_y;

			rotated_x = (rot_surf_edge - src_surf_w) / 2 + rotated_x;
			rotated_y = (rot_surf_edge - src_surf_h) / 2 + rotated_y;

			//===================================================

			put_pixel(rotated_surface, rotated_x, rotated_y, pixel);
		}
	}

	SDL_SaveBMP(rotated_surface, SAVED_IMG_NAME_R);
	SDL_FreeSurface(src_surface);
	SDL_FreeSurface(rotated_surface);
}