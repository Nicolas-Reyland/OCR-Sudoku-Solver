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

/*
static const int g_blur_kernel_5[5][5] = {{1, 4, 7, 4, 1},
										{4, 16, 26, 16, 4},
										{7, 26, 41, 26, 7},
										{4, 16, 26, 16, 4},
										{1, 4, 7, 4, 1}};

#define G_BLUR_KERNEL_SIZE 5
#define G_BLUR_KERNEL_SUM 273
*/

#define G_STD_DEVIATION 1
#define G_BOX_BLUR_NB 3

#define S_BINARISATION_SIZE 61

// K takes a value from the interval [0.2, 0.5]
#define K_BINARISATION 0.3f

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

unsigned long long int **create_integral_image(SDL_Surface *surface, 
					unsigned int margin, II_TYPE ii_type)
{
	Uint32 pixel;
	Uint8 composant;

	size_t surface_width = surface->w;
	size_t surface_height = surface->h;

	size_t ii_width = surface_width + margin;
	size_t ii_height = surface_height + margin;

	int half_margin = margin / 2;

	// 2D unsigned long int array allocation
	unsigned long long int **integral_image = 
	(unsigned long long int**)calloc(ii_width, sizeof(unsigned long long int*));

	for(size_t i = 0; i < ii_width; i++)
	{
		integral_image[i] = 
		(unsigned long long int*)calloc(ii_height, sizeof(unsigned long long int));
	}

	// Represents pixel (0,0) in the integral image
	size_t ii_x = half_margin + margin % 2;
	size_t ii_y = half_margin + margin % 2;

	// Pixel (0,0)
	pixel = get_pixel(surface, 0, 0);
	SDL_GetRGB(pixel, surface->format, 
					&composant, &composant, &composant);
	integral_image[ii_x][ii_y] = integral_image_value(ii_type, composant);

	// First row (i, 0)
	for (size_t i = 1; i < surface_width; i++)
	{
		pixel = get_pixel(surface, i, 0);
		SDL_GetRGB(pixel, surface->format, 
					&composant, &composant, &composant);

		integral_image[i + ii_x][ii_y] = 
		integral_image[i + ii_x - 1][ii_y] 
		+ integral_image_value(ii_type, composant);
	}

	// till the end of the array
	for (size_t i = surface_width + ii_x; i < ii_width; i++)
	{
		integral_image[i][ii_y] = integral_image[i - 1][ii_y];
	}

	// First column (0, j)
	for (size_t j = 1; j < surface_height; j++)
	{
		pixel = get_pixel(surface, 0, j);
		SDL_GetRGB(pixel, surface->format, 
					&composant, &composant, &composant);

		integral_image[ii_x][j + ii_y] = 
		integral_image[ii_x][j + ii_y - 1] 
		+ integral_image_value(ii_type, composant);
	}

	// till the end of the array
	for (size_t j = surface_height + ii_y; j < ii_height; j++)
	{
		integral_image[ii_x][j] = integral_image[ii_x][j - 1];
	}

	// All the remaining pixels of the source image
	for (size_t i = 1; i < surface_width; i++)
	{
		for (size_t j = 1; j < surface_height; j++)
		{
			pixel = get_pixel(surface, i, j);
			SDL_GetRGB(pixel, surface->format, 
					&composant, &composant, &composant);

			integral_image[i + ii_x][j + ii_y] =
					integral_image_value(ii_type, composant) 
					+ integral_image[i + ii_x][j + ii_y - 1] 
					+ integral_image[i + ii_x - 1][j + ii_y]
					- integral_image[i + ii_x - 1][j + ii_y - 1];

			//printf("Value ii -> %lu\n", integral_image[i + ii_x][j + ii_y]);
		}
	}

	// Remaining right pixels of integral image 
	size_t max_height = ii_y + surface_height;
	for (size_t i = surface_width + ii_x; i < ii_width; i++)
	{
		for (size_t j = ii_y; j < max_height; j++)
		{
			integral_image[i][j] = 
					integral_image[i][j - 1] 
					+ integral_image[i - 1][j]
					- integral_image[i - 1][j - 1];
		}
	}

	// Remaining bottom pixels of integral image 
	for (size_t i = ii_x; i < ii_width; i++)
	{
		for (size_t j = ii_y + surface_height; j < ii_height; j++)
		{
			integral_image[i][j] = 
					integral_image[i][j - 1] 
					+ integral_image[i - 1][j]
					- integral_image[i - 1][j - 1];
		}
	}

	return integral_image;
}

void free_integral_image(unsigned long long int **integral_image, size_t width)
{
	for(size_t i = 0; i < width; i++)
	{
		free(integral_image[i]);
		integral_image[i] = NULL;
	}

	free(integral_image);
}

unsigned long long int integral_image_value(II_TYPE ii_type, Uint8 composant)
{
	switch(ii_type)
	{
		case MEAN:
			return composant;
		case STD_DEVIATION:
			return composant * composant;
		default:
			fprintf(stderr, "Invalid II_TYPE\n");
			exit(EXIT_FAILURE);
			return 0;
	}
}

void grayscale(Uint8 *r, Uint8 *g, Uint8 *b)
{
	Uint8 grayscale;
	/* Mean method
	grayscale = *r / 3 + *g / 3 + *b / 3;
	*/

	///* BGR2GRAY -> opencv method
	//grayscale = roundf(*r * 0.2126f + *g * 0.7152f + *b * 0.0722f);
	grayscale = *g;
	//*/

	*r = grayscale;
	*g = grayscale;
	*b = grayscale;
}

int* boxes_sizes_for_gauss(float std_deviation, int nb_boxes)
{
	int *boxes_sizes = (int*)calloc(nb_boxes, sizeof(int));

	double w_ideal = sqrt((12 * std_deviation * std_deviation / nb_boxes) + 1);
	int wl = floor(w_ideal);
	if (wl % 2 == 0)
		wl -= 1;
	int wu = wl + 2;

	double m_ideal = 
	(12 * std_deviation * std_deviation - nb_boxes * wl * wl 
		- 4 * nb_boxes * wl - 3 * nb_boxes) / (-4 * wl - 4);
	int m = round(m_ideal);

	for (int i = 0; i < nb_boxes; i++)
	{
		if (i < m)
			boxes_sizes[i] = wl;
		else
			boxes_sizes[i] = wu;
	}

	return boxes_sizes;
}

void box_blur(SDL_Surface *src_surface, SDL_Surface *blurred_surface, 
										unsigned int bb_size)
{
	size_t surface_w = src_surface->w;
	size_t surface_h = src_surface->h;

	SDL_Surface *intermediate_surface = SDL_CreateRGBSurface (0, 
		surface_w, surface_h, 32, 0, 0, 0, 0);

	hotizontal_box_blur(src_surface, intermediate_surface, bb_size / 2);
	vertical_box_blur(intermediate_surface, blurred_surface, bb_size / 2);

	SDL_FreeSurface(intermediate_surface);
}

void hotizontal_box_blur(SDL_Surface *src_surface, 
	SDL_Surface *h_blurred_surface, int box_radius)
{
	Uint32 pixel;
	Uint8 composant;

	long int surface_w = src_surface->w;
	long int surface_h = src_surface->h;

	for (long int i = 0; i < surface_w; i++)
	{
		for (long int j = 0; j < surface_h; j++)
		{
			unsigned int composant_sum = 0;
			
			for (int k = -box_radius; k < (box_radius + 1); k++)
			{
				if ((i + k) >= 0 && (i + k) < surface_w)
				{
					pixel = get_pixel(src_surface, i + k, j);
					SDL_GetRGB(pixel, src_surface->format, 
					&composant, &composant, &composant);

					composant_sum += composant;
				}
			}

			composant_sum /= (box_radius + box_radius + 1);
			pixel = SDL_MapRGB(h_blurred_surface->format, 
					composant_sum, composant_sum, composant_sum);
			put_pixel(h_blurred_surface, i, j, pixel);
		}
	}
}

void vertical_box_blur(SDL_Surface *src_surface, 
	SDL_Surface *v_blurred_surface, int box_radius)
{
	Uint32 pixel;
	Uint8 composant;

	long int surface_w = src_surface->w;
	long int surface_h = src_surface->h;

	for (long int i = 0; i < surface_w; i++)
	{
		for (long int j = 0; j < surface_h; j++)
		{
			unsigned int composant_sum = 0;

			for (int k = -box_radius; k < (box_radius + 1); k++)
			{
				if ((j + k) >= 0 && (j + k) < surface_h)
				{
					pixel = get_pixel(src_surface, i, j + k);
					SDL_GetRGB(pixel, src_surface->format, 
					&composant, &composant, &composant);

					composant_sum += composant;
				}
			}

			composant_sum /= (box_radius + box_radius + 1);
			pixel = SDL_MapRGB(v_blurred_surface->format, 
					composant_sum, composant_sum, composant_sum);
			put_pixel(v_blurred_surface, i, j, pixel);
		}
	}

}

SDL_Surface* gaussian_blur(SDL_Surface *src_surface)
{
	size_t surface_w = src_surface->w;
	size_t surface_h = src_surface->h;

	int *boxes_sizes = boxes_sizes_for_gauss(G_STD_DEVIATION, 
														G_BOX_BLUR_NB);

	SDL_Surface *blurred_surface = SDL_CreateRGBSurface (0, 
		surface_w, surface_h, 32, 0, 0, 0, 0);

	SDL_Surface *intermediate_surface = SDL_CreateRGBSurface (0, 
		surface_w, surface_h, 32, 0, 0, 0, 0);

	if (G_BOX_BLUR_NB > 0)
		box_blur(src_surface, blurred_surface, boxes_sizes[0]);

	for (int i = 2; i < G_BOX_BLUR_NB; i += 2)
	{
		box_blur(blurred_surface, intermediate_surface, boxes_sizes[i - 1]);
		box_blur(intermediate_surface, blurred_surface, boxes_sizes[i]);
	}

	SDL_FreeSurface(intermediate_surface);

	return blurred_surface;
}

double sauvola_binarisation(unsigned long long int **mean_ii, 
							unsigned long long int **std_deviation_ii, 
							int i, int j)
{
	int s_bin_size_pow_2 = S_BINARISATION_SIZE * S_BINARISATION_SIZE;

	double mean = 0.0f;
	double std_deviation = 0.0f;
	double threshold = 0.0f;

	unsigned long long int squared_pixel_sum = 0;

	int margin = S_BINARISATION_SIZE + 1;
	int half_margin = margin / 2;
	int half_bin_size = S_BINARISATION_SIZE / 2;

	// Represents pixel (0,0) in the integral image
	size_t ii_x = half_margin + margin % 2;
	size_t ii_y = ii_x;

	//printf("ii_x -> %zu\n", ii_x);
	//printf("i - half_bin_size - 1 -> %d\n", i - half_bin_size - 1);

	//printf("Value ii -> %lu\n", mean_ii[ii_x][src_surface_h + margin - 1]);

	//--------------------------------Mean--------------------------------

	unsigned long long int top_left_ii_val = 
		mean_ii[ii_x + i - half_bin_size - 1][ii_y + j - half_bin_size - 1];

	unsigned long long int top_right_ii_val = 
	mean_ii[ii_x + i + half_bin_size][ii_y + j - half_bin_size - 1];

	unsigned long long int bottom_right_ii_val = 
	mean_ii[ii_x + i + half_bin_size][ii_y + j + half_bin_size];

	unsigned long long int bottom_left_ii_val = 
	mean_ii[ii_x + i - half_bin_size - 1][ii_y + j + half_bin_size];

	mean = bottom_right_ii_val - bottom_left_ii_val 
			- top_right_ii_val + top_left_ii_val;

	mean /= s_bin_size_pow_2;

	//-------------------------Standard Deviation-------------------------

	top_left_ii_val = 
	std_deviation_ii[ii_x + i - half_bin_size - 1][ii_y + j - half_bin_size - 1];

	top_right_ii_val = 
	std_deviation_ii[ii_x + i + half_bin_size][ii_y + j - half_bin_size - 1];

	bottom_right_ii_val = 
	std_deviation_ii[ii_x + i + half_bin_size][ii_y + j + half_bin_size];

	bottom_left_ii_val = 
	std_deviation_ii[ii_x + i - half_bin_size - 1][ii_y + j + half_bin_size];

	squared_pixel_sum = bottom_right_ii_val - bottom_left_ii_val 
			- top_right_ii_val + top_left_ii_val;

	std_deviation = squared_pixel_sum - (mean * mean * s_bin_size_pow_2);
	std_deviation /= s_bin_size_pow_2;
	std_deviation = sqrt(std_deviation);

	threshold = mean * (1 + K_BINARISATION * ((std_deviation / R) - 1));
	//threshold = mean * ((1 + K_BINARISATION * std_deviation)/(R - 1));

	return threshold;
}

void image_process(char *path)
{
	SDL_Surface *surface = NULL;
	Uint32 pixel;
	Uint8 r, g, b;
	double threshold;

	init_sdl();
	surface = load_image(path);

	size_t surf_width = surface->w;
	size_t surf_height = surface->h;

	//===================================================
	//****************Grayscale process******************
	//===================================================

	for(size_t i = 0; i < surf_width; i++)
	{
		for (size_t j = 0; j < surf_height;j++)
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

	SDL_Surface *blurred_surface = gaussian_blur(surface);

	if(blurred_surface == NULL)
	{
	    errx(EXIT_FAILURE, "Failed to create blurred surface.");
	}

	SDL_FreeSurface(surface);
	SDL_SaveBMP(blurred_surface, SAVED_IMG_NAME_BL);

	//===================================================
	//***********Sauvola binarisation process************
	//===================================================

	// Transparency is not activated
	SDL_Surface *binarised_surface = SDL_CreateRGBSurface (0, 
		surf_width, surf_height, 32, 0, 0, 0, 0);

	/*
	if(binarised_surface == NULL)
	{
	    printf("Failed to create surface -> %s\n", SDL_GetError());
	}
	*/

	int margin = S_BINARISATION_SIZE + 1;

	unsigned long long int **mean_ii = 
	create_integral_image(blurred_surface, margin, MEAN);

	unsigned long long int **std_deviation_ii = 
	create_integral_image(blurred_surface, margin, STD_DEVIATION);

	//size_t ii_x = (margin / 2) + margin % 2;
	//size_t ii_y = ii_x;

	//printf("Value ii -> %lu\n", std_deviation_ii[ii_x + surf_width - 2][ii_y + surf_height - 2] + 2);
	//printf("%lu\n", sizeof(unsigned long int));

	for(size_t i = 0; i < surf_width; i++)
	{
		for (size_t j = 0; j < surf_height;j++)
		{
			pixel = get_pixel(blurred_surface, i, j);
			SDL_GetRGB(pixel, blurred_surface->format, &r, &g, &b);

			threshold = sauvola_binarisation(mean_ii, std_deviation_ii, i, j);

			threshold = round(threshold);

			//r = r >= threshold;
			//r = r * 255;
			
			if (r > threshold)
				r = 255;
			else
				r = 0;

			pixel = SDL_MapRGB(binarised_surface->format, r, r, r);
			put_pixel(binarised_surface, i, j, pixel);
		}
	}

	SDL_SaveBMP(binarised_surface, SAVED_IMG_NAME_BI);
	
	SDL_FreeSurface(blurred_surface);
	SDL_FreeSurface(binarised_surface);

	free_integral_image(mean_ii, surf_width + margin);
	free_integral_image(std_deviation_ii, surf_width + margin);

	// To avoid to overwrite memory by reaccessing it
	mean_ii = NULL;
	std_deviation_ii = NULL;
}

// The angle given is in degrees
void rotate_image(char *path, double angle)
{
	SDL_Surface *src_surface = NULL;
	SDL_Surface *rotated_surface = NULL;
	Uint32 pixel;
	Uint8 r, g, b;

	double min_limit = 31 * PI / 36;
	double max_limit = 41 * PI / 36;

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

	double angle_abs_val = fabs(angle);

	if (min_limit < angle_abs_val && angle_abs_val < max_limit)
	{
		for(int i = 0; i < src_surf_w; i++)
		{
			for (int j = 0; j < src_surf_h; j++)
			{
				pixel = get_pixel(src_surface, i, j);
				SDL_GetRGB(pixel, src_surface->format, &r, &g, &b);
				pixel = SDL_MapRGB(rotated_surface->format, r, g, b);
				
				x = i - half_src_surf_w;
				y = half_src_surf_h - j;
				
				//===================================================
				//****************Rotation-Formula 1*****************
				//===================================================

				rotated_x = round(x * cos(angle) - y * sin(angle));
				rotated_y = round(x * sin(angle) + y * cos(angle));

				//===================================================

				//printf("rotated_x = %d | rotated_y = %d\n", rotated_x, rotated_y);

				rotated_x = rotated_x + half_src_surf_w;
				rotated_y = half_src_surf_h - rotated_y;

				rotated_x = (rot_surf_edge - src_surf_w) / 2 + rotated_x;
				rotated_y = (rot_surf_edge - src_surf_h) / 2 + rotated_y;

				put_pixel(rotated_surface, rotated_x, rotated_y, pixel);

			}
		}
	}
	else // tan(pi/2) is undefined
	{
		for(int i = 0; i < src_surf_w; i++)
		{
			for (int j = 0; j < src_surf_h; j++)
			{
				pixel = get_pixel(src_surface, i, j);
				SDL_GetRGB(pixel, src_surface->format, &r, &g, &b);
				pixel = SDL_MapRGB(rotated_surface->format, r, g, b);
				
				x = i - half_src_surf_w;
				y = half_src_surf_h - j;
				
				
				//===================================================
				//****************Rotation-Formula 2*****************
				//===================================================

				// Shear 1
				rotated_x = round(x-y*tan(angle/2));
				rotated_y = y;

				// Shear 2
				rotated_y = round(rotated_x*sin(angle)+rotated_y);

				// Shear 3
				rotated_x = round(rotated_x-rotated_y*tan(angle/2));

				//===================================================

				rotated_x = rotated_x + half_src_surf_w;
				rotated_y = half_src_surf_h - rotated_y;

				rotated_x = (rot_surf_edge - src_surf_w) / 2 + rotated_x;
				rotated_y = (rot_surf_edge - src_surf_h) / 2 + rotated_y;

				put_pixel(rotated_surface, rotated_x, rotated_y, pixel);
			}
		}
	}

	SDL_SaveBMP(rotated_surface, SAVED_IMG_NAME_R);
	SDL_FreeSurface(src_surface);
	SDL_FreeSurface(rotated_surface);
}