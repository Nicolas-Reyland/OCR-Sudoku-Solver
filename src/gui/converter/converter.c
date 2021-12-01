// converter.c

#include "converter.h"

void __converter(char* filepath, double* converted_cell);

void converter(char* path, double** converted_cells, Cell** cells_position)
{
	verbose("Reading directory at path: %s\n", path);
	// I don't actually init sdl because I assume
	// it has already been initialized by grid_description
	DIR* directory;
	unsigned int index = 0;
	directory = opendir(path);
	struct dirent *dir;
	unsigned int num_files = count_files(path);
	if (directory == NULL)
		errx(1, "CONVERTER Error: There is no directory.");
	while((dir = readdir(directory)) != NULL)
	{
		verbose("index: %3d ", index);

		if (dir->d_type == DT_REG)
		{
			//getting path for file
			char* buffer = mem_calloc(1000, sizeof(char));
			sprintf(buffer, "%s%s",path, dir->d_name);
			verbose("%s\n", buffer);

			//converting...
			//allocatting memory to the double array
			converted_cells[index] = mem_calloc(IMG_SIZE, sizeof(double));
			__converter(buffer, converted_cells[index]);

			// parsing cell name into a struct
			Cell* cell = mem_malloc(sizeof(Cell));
			int x=0, y = 0;

			sscanf(dir->d_name, "cell_%d_%d", &x, &y);

			cell->x = x;
			cell->y = y;

			cells_position[index] = cell;

			//freeing the string buffer
			mem_free(buffer);

		}
		else
		{
			//symlinks are filtered
			verbose("Not a file: %s\n", dir->d_name);
		}

		if (++index == num_files)
			break;
	}
	closedir(directory);
}

unsigned int count_files(char* path)
{
	verbose("Reading directory at path: %s\n", path);

	DIR* directory;
	directory = opendir(path);
	if (directory == NULL)
		errx(1, "CONVERTER Error: There is no directory.");

	unsigned int counter = 0;
	struct dirent *dir;

	while((dir = readdir(directory)) != NULL)
	{
		if (dir->d_type == DT_REG)
			counter++;
	}
	return counter;
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
