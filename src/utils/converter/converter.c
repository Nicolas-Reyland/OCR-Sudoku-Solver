#include "converter.h"

void __converter(char* filepath, double* converted_cell);

void converter(char* path, double** converted_cells, Cell** cells_position)
{
	// I don't actually init sdl because I assume 
	// it has already been initialized by grid_description
	DIR* directory;
	unsigned int index = 0;
	directory = opendir(path);
	struct dirent *dir;
	if (directory == NULL)
		errx(1, "CONVERTER Error: There is no directory.");
	printf("Reading directory at path: %s\n", path);
	while((dir = readdir(directory)) != NULL)
	{
		printf("index: %3d ", index);
		if (dir->d_type == DT_REG)
		{
			//getting path for file
			char* buffer = calloc(1000, sizeof(char));
			sprintf(buffer, "%s/%s",path, dir->d_name);
			printf("%s\n", buffer);

			//converting...
			//allocatting memory to the double array
			converted_cells[index] = calloc(IMG_SIZE, sizeof(double));
			__converter(buffer, converted_cells[index]);

			// parsing cell name into a struct
			Cell* cell = malloc(sizeof(Cell));
			int x=0, y = 0;

			sscanf(dir->d_name, "cell_%d_%d", &x, &y);

			cell->x = x;
			cell->y = y;

			cells_position[index] = cell;

			//freeing the string buffer
			free(buffer);
			
		}
		else
		{
			//symlinks are filtered
			printf("Not a file: %s\n", dir->d_name);
		}

		
		index++;
	}
	closedir(directory);
}

//converting image into a double array
void __converter(char* filepath, double* converted_cell)
{
	SDL_Surface* image_surface = cv_load_image(filepath);

	Uint32 pixel;
	for (int i = 0; i < image_surface->w; i++)
	{
		for (int j = 0; j < image_surface->h; j++)
		{
			pixel = cv_get_pixel(image_surface,i,j);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			double value = (double)r / 255;

			converted_cell[i * 28 + j] = value;
		}
	}
	SDL_FreeSurface(image_surface);
}
