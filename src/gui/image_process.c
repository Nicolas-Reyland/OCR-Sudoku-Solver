#include "gui/image_process.h"

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

static const float g_blur_kernel_5[7] = {0.006, 0.061, 0.242, 0.383,
                                         0.242, 0.061, 0.006};

#define G_BLUR_KERNEL_SIZE 7

//#define S_BINARISATION_SIZE 61
#define S_BINARISATION_SIZE 61

// K takes a value from the interval [0.2, 0.5]
long double K_BINARISATION = 0.2f;

// R is the max value of the standard deviation of an image in grayscale
#define R 128

void init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO != 0))
        errx(1, "Failed to initialize SDL -> %s.\n", SDL_GetError());
}

void init_sdl_ttf() {
    if (TTF_Init() != 0)
        errx(1, "Failed to initialize SDL_TTF -> %s.\n", SDL_GetError());
}

SDL_Surface *load_image(char *path) {
    SDL_Surface *image;

    image = IMG_Load(path);
    if (!image)
        errx(3, "Failed to load %s: %s", path, IMG_GetError());

    return image;
}

void color_surface(SDL_Surface *surface, Uint32 pixel) {
    unsigned int surf_width = surface->w;
    unsigned int surf_height = surface->h;

    for (unsigned int i = 0; i < surf_width; i++) {
        for (unsigned int j = 0; j < surf_height; j++) {
            put_pixel(surface, i, j, pixel);
        }
    }
}

unsigned long long int **create_integral_image(SDL_Surface *surface,
                                               unsigned int margin,
                                               II_TYPE ii_type) {
    Uint32 pixel;
    Uint8 composant;

    size_t surface_width = surface->w;
    size_t surface_height = surface->h;

    size_t ii_width = surface_width + margin;
    size_t ii_height = surface_height + margin;

    int half_margin = margin / 2;

    // 2D unsigned long int array allocation
    unsigned long long int **integral_image = (unsigned long long int **)calloc(
        ii_width, sizeof(unsigned long long int *));

    for (size_t i = 0; i < ii_width; i++) {
        integral_image[i] = (unsigned long long int *)calloc(
            ii_height, sizeof(unsigned long long int));
    }

    // Represents pixel (0,0) in the integral image
    size_t ii_x = half_margin + margin % 2;
    size_t ii_y = half_margin + margin % 2;

    // Pixel (0,0)
    pixel = get_pixel(surface, 0, 0);
    SDL_GetRGB(pixel, surface->format, &composant, &composant, &composant);
    integral_image[ii_x][ii_y] = integral_image_value(ii_type, composant);

    // First row (i, 0)
    for (size_t i = 1; i < surface_width; i++) {
        pixel = get_pixel(surface, i, 0);
        SDL_GetRGB(pixel, surface->format, &composant, &composant, &composant);

        integral_image[i + ii_x][ii_y] =
            integral_image[i + ii_x - 1][ii_y] +
            integral_image_value(ii_type, composant);
    }

    // till the end of the array
    for (size_t i = surface_width + ii_x; i < ii_width; i++) {
        integral_image[i][ii_y] = integral_image[i - 1][ii_y];
    }

    // First column (0, j)
    for (size_t j = 1; j < surface_height; j++) {
        pixel = get_pixel(surface, 0, j);
        SDL_GetRGB(pixel, surface->format, &composant, &composant, &composant);

        integral_image[ii_x][j + ii_y] =
            integral_image[ii_x][j + ii_y - 1] +
            integral_image_value(ii_type, composant);
    }

    // till the end of the array
    for (size_t j = surface_height + ii_y; j < ii_height; j++) {
        integral_image[ii_x][j] = integral_image[ii_x][j - 1];
    }

    // All the remaining pixels of the source image
    for (size_t i = 1; i < surface_width; i++) {
        for (size_t j = 1; j < surface_height; j++) {
            pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &composant, &composant,
                       &composant);

            integral_image[i + ii_x][j + ii_y] =
                integral_image_value(ii_type, composant) +
                integral_image[i + ii_x][j + ii_y - 1] +
                integral_image[i + ii_x - 1][j + ii_y] -
                integral_image[i + ii_x - 1][j + ii_y - 1];

            // printf("Value ii -> %lu\n", integral_image[i + ii_x][j + ii_y]);
        }
    }

    // Remaining right pixels of integral image
    size_t max_height = ii_y + surface_height;
    for (size_t i = surface_width + ii_x; i < ii_width; i++) {
        for (size_t j = ii_y; j < max_height; j++) {
            integral_image[i][j] = integral_image[i][j - 1] +
                                   integral_image[i - 1][j] -
                                   integral_image[i - 1][j - 1];
        }
    }

    // Remaining bottom pixels of integral image
    for (size_t i = ii_x; i < ii_width; i++) {
        for (size_t j = ii_y + surface_height; j < ii_height; j++) {
            integral_image[i][j] = integral_image[i][j - 1] +
                                   integral_image[i - 1][j] -
                                   integral_image[i - 1][j - 1];
        }
    }

    return integral_image;
}

void free_integral_image(unsigned long long int **integral_image,
                         size_t width) {
    for (size_t i = 0; i < width; i++) {
        free(integral_image[i]);
        integral_image[i] = NULL;
    }

    free(integral_image);
}

unsigned long long int integral_image_value(II_TYPE ii_type, Uint8 composant) {
    switch (ii_type) {
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

void grayscale(Uint8 *r, Uint8 *g, Uint8 *b) {
    Uint8 grayscale;
    /* Mean method
    grayscale = (double)*r / 3 + (double)*g / 3 + (double)*b / 3;
    */

    // BGR2GRAY -> opencv method
    // grayscale = (Uint8)roundf((double)*r * 0.2126f + (double)*g * 0.7152f +
    // (double)*b * 0.0722f); grayscale = *g;

    // Luminance formula
    grayscale = (Uint8)roundf((double)*r * 0.299f + (double)*g * 0.587f +
                              (double)*b * 0.114f);

    *r = grayscale;
    *g = grayscale;
    *b = grayscale;
}

// Calculates the value of a pixel after applying a gaussian blur process
SDL_Surface *gaussian_blur(SDL_Surface *src_surface, int is_normalized,
                           int min_cmp_norm, int max_cmp_norm) {
    Uint32 pixel;
    Uint8 composant;

    int src_surface_w = src_surface->w;
    int src_surface_h = src_surface->h;

    unsigned int *intermediate_p_array = (unsigned int *)calloc(
        src_surface_w * src_surface_h, sizeof(unsigned int));

    float composant_sum = 0.0f;

    int mid_coordinates = G_BLUR_KERNEL_SIZE / 2;

    // We take into consideration the normalization
    if (is_normalized == 1) {
        for (int j = 0; j < src_surface_h; j++) {
            for (int i = 0; i < src_surface_w; i++) {
                for (int k = 0; k < G_BLUR_KERNEL_SIZE; k++) {
                    int current_i = i - mid_coordinates + k;

                    if (current_i >= 0 && current_i < src_surface_w) {
                        pixel = get_pixel(src_surface, current_i, j);
                        SDL_GetRGB(pixel, src_surface->format, &composant,
                                   &composant, &composant);

                        composant = (Uint8)round(
                            (double)255 *
                            ((double)composant - (double)min_cmp_norm) /
                            ((double)max_cmp_norm - (double)min_cmp_norm));

                        composant_sum += g_blur_kernel_5[k] * composant;
                    }
                }

                intermediate_p_array[i * src_surface_h + j] =
                    (unsigned int)(roundf(composant_sum));

                composant_sum = 0.0f;
            }
        }
    } else // Normalization is not taken into consideration
    {
        for (int j = 0; j < src_surface_h; j++) {
            for (int i = 0; i < src_surface_w; i++) {
                for (int k = 0; k < G_BLUR_KERNEL_SIZE; k++) {
                    int current_i = i - mid_coordinates + k;

                    if (current_i >= 0 && current_i < src_surface_w) {
                        pixel = get_pixel(src_surface, current_i, j);
                        SDL_GetRGB(pixel, src_surface->format, &composant,
                                   &composant, &composant);

                        composant_sum += g_blur_kernel_5[k] * composant;
                    }
                }

                intermediate_p_array[i * src_surface_h + j] =
                    (unsigned int)(roundf(composant_sum));

                composant_sum = 0.0f;
            }
        }
    }

    SDL_Surface *blurred_surface =
        SDL_CreateRGBSurface(0, src_surface_w, src_surface_h, 32, 0, 0, 0, 0);

    for (int i = 0; i < src_surface_w; i++) {
        for (int j = 0; j < src_surface_h; j++) {
            for (int k = 0; k < G_BLUR_KERNEL_SIZE; k++) {
                int current_j = j - mid_coordinates + k;

                if (current_j >= 0 && current_j < src_surface_h) {
                    composant =
                        intermediate_p_array[i * src_surface_h + current_j];

                    composant_sum += g_blur_kernel_5[k] * composant;
                }
            }

            composant_sum = (Uint8)(roundf(composant_sum));

            pixel = SDL_MapRGB(blurred_surface->format, composant_sum,
                               composant_sum, composant_sum);
            put_pixel(blurred_surface, i, j, pixel);

            composant_sum = 0.0f;
        }
    }

    free(intermediate_p_array);

    return blurred_surface;
}

long double sauvola_binarisation(unsigned long long int **mean_ii,
                                 unsigned long long int **std_deviation_ii,
                                 int i, int j) {
    int s_bin_size_pow_2 = S_BINARISATION_SIZE * S_BINARISATION_SIZE;

    long double mean = 0.0f;
    long double std_deviation = 0.0f;
    long double threshold = 0.0f;

    unsigned long long int pixel_sum = 0;
    unsigned long long int squared_pixel_sum = 0;

    int margin = S_BINARISATION_SIZE + 1;
    int half_margin = margin / 2;
    int half_bin_size = S_BINARISATION_SIZE / 2;

    // Represents pixel (0,0) in the integral image
    size_t ii_x = half_margin + margin % 2;
    size_t ii_y = ii_x;

    // printf("ii_x -> %zu\n", ii_x);
    // printf("i - half_bin_size - 1 -> %d\n", i - half_bin_size - 1);

    // printf("Value ii -> %lu\n", mean_ii[ii_x][src_surface_h + margin - 1]);

    //--------------------------------Mean--------------------------------

    unsigned long long int top_left_ii_val =
        mean_ii[ii_x + i - half_bin_size - 1][ii_y + j - half_bin_size - 1];

    unsigned long long int top_right_ii_val =
        mean_ii[ii_x + i + half_bin_size][ii_y + j - half_bin_size - 1];

    unsigned long long int bottom_right_ii_val =
        mean_ii[ii_x + i + half_bin_size][ii_y + j + half_bin_size];

    unsigned long long int bottom_left_ii_val =
        mean_ii[ii_x + i - half_bin_size - 1][ii_y + j + half_bin_size];

    pixel_sum = bottom_right_ii_val - bottom_left_ii_val - top_right_ii_val +
                top_left_ii_val;

    mean = (long double)pixel_sum / (long double)s_bin_size_pow_2;

    //-------------------------Standard Deviation-------------------------

    top_left_ii_val = std_deviation_ii[ii_x + i - half_bin_size - 1]
                                      [ii_y + j - half_bin_size - 1];

    top_right_ii_val = std_deviation_ii[ii_x + i + half_bin_size]
                                       [ii_y + j - half_bin_size - 1];

    bottom_right_ii_val =
        std_deviation_ii[ii_x + i + half_bin_size][ii_y + j + half_bin_size];

    bottom_left_ii_val = std_deviation_ii[ii_x + i - half_bin_size - 1]
                                         [ii_y + j + half_bin_size];

    squared_pixel_sum = bottom_right_ii_val - bottom_left_ii_val -
                        top_right_ii_val + top_left_ii_val;

    /*
    std_deviation = ((long double)squared_pixel_sum) - (mean * mean * ((long
    double)s_bin_size_pow_2)); if (std_deviation < 0) errx(1, "NEGATIVE VALUE
    (OLD)\n"); std_deviation /= (long double)s_bin_size_pow_2; std_deviation =
    sqrtl(std_deviation);
    */

    ///*
    std_deviation =
        ((long double)squared_pixel_sum / (long double)s_bin_size_pow_2) -
        (mean * mean);
    std_deviation = sqrtl(std_deviation);
    //*/

    threshold = mean * (1 + (long double)K_BINARISATION *
                                ((std_deviation / (long double)R) - 1));
    // threshold = mean * ((1 + K_BINARISATION * std_deviation)/(R - 1));

    return threshold;
}

void image_process(char *path, int is_normalized) {
    SDL_Surface *surface = NULL;

    Uint32 pixel;
    Uint8 r, g, b;

    int min_composant = 255, max_composant = 0;

    long double threshold;

    if (is_normalized)
        K_BINARISATION = 0.05f;
    else
        K_BINARISATION = 0.2f;

    surface = load_image(path);

    size_t surf_width = surface->w;
    size_t surf_height = surface->h;

    //===================================================
    //****************Grayscale process******************
    //===================================================

    for (size_t i = 0; i < surf_width; i++) {
        for (size_t j = 0; j < surf_height; j++) {
            pixel = get_pixel(surface, i, j);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);

            grayscale(&r, &g, &b);

            pixel = SDL_MapRGB(surface->format, r, r, r);
            put_pixel(surface, i, j, pixel);

            if ((int)r > max_composant)
                max_composant = (int)r;

            if ((int)r < min_composant)
                min_composant = (int)r;
        }
    }

    SDL_SaveBMP(surface, SAVED_IMG_NAME_G);

    //===================================================
    //******************G.Blur process*******************
    //===================================================

    // Transparency is not activated
    SDL_Surface *blurred_surface =
        gaussian_blur(surface, is_normalized, min_composant, max_composant);

    if (blurred_surface == NULL) {
        printf("Failed to create gaussian blurred surface\n");
    }

    SDL_FreeSurface(surface);
    SDL_SaveBMP(blurred_surface, SAVED_IMG_NAME_BL);

    //===================================================
    //***********Sauvola binarisation process************
    //===================================================

    // Transparency is not activated
    SDL_Surface *binarised_surface =
        SDL_CreateRGBSurface(0, surf_width, surf_height, 32, 0, 0, 0, 0);

    if (binarised_surface == NULL) {
        printf("Failed to create surface -> %s\n", SDL_GetError());
    }

    int margin = S_BINARISATION_SIZE + 1;

    unsigned long long int **mean_ii =
        create_integral_image(blurred_surface, margin, MEAN);

    unsigned long long int **std_deviation_ii =
        create_integral_image(blurred_surface, margin, STD_DEVIATION);

    // size_t ii_x = (margin / 2) + margin % 2;
    // size_t ii_y = ii_x;

    // printf("Value ii -> %lu\n", std_deviation_ii[ii_x + surf_width - 2][ii_y
    // + surf_height - 2] + 2); printf("%lu\n", sizeof(unsigned long int));

    for (size_t i = 0; i < surf_width; i++) {
        for (size_t j = 0; j < surf_height; j++) {
            pixel = get_pixel(blurred_surface, i, j);
            SDL_GetRGB(pixel, blurred_surface->format, &r, &g, &b);

            threshold = sauvola_binarisation(mean_ii, std_deviation_ii, i, j);

            threshold = roundl(threshold);
            // printf("%Lf\n", threshold);

            // r = r >= threshold;
            // r = r * 255;

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

    free_integral_image(mean_ii, surf_width + margin);
    free_integral_image(std_deviation_ii, surf_width + margin);

    // Avoid to overwrite memory by reaccessing it
    mean_ii = NULL;
    std_deviation_ii = NULL;
}

// The angle given is in degrees
void rotate_image(char *path, double angle) {
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

    src_surface = load_image(path);

    int src_surf_w = src_surface->w;
    int src_surf_h = src_surface->h;

    int half_src_surf_w = src_surf_w / 2;
    int half_src_surf_h = src_surf_h / 2;

    int rot_surf_edge;

    if (fmod(angle, (double)PI / 2) == 0) {
        if (src_surf_w > src_surf_h)
            rot_surf_edge = src_surf_w + 1;
        else
            rot_surf_edge = src_surf_h + 1;
    } else {
        rot_surf_edge =
            (int)sqrt(src_surf_w * src_surf_w + src_surf_h * src_surf_h) + 1;
    }

    // Transparency is not activated
    // Creation of a white surface
    rotated_surface =
        SDL_CreateRGBSurface(0, rot_surf_edge, rot_surf_edge, 32, 0, 0, 0, 0);

    color_surface(rotated_surface,
                  SDL_MapRGB(rotated_surface->format, 255, 255, 255));

    double angle_abs_val = fabs(angle);

    if (min_limit < angle_abs_val && angle_abs_val < max_limit) {
        for (int i = 0; i < src_surf_w; i++) {
            for (int j = 0; j < src_surf_h; j++) {
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

                // printf("rotated_x = %d | rotated_y = %d\n", rotated_x,
                // rotated_y);

                rotated_x = rotated_x + half_src_surf_w;
                rotated_y = half_src_surf_h - rotated_y;

                rotated_x = (rot_surf_edge - src_surf_w) / 2 + rotated_x;
                rotated_y = (rot_surf_edge - src_surf_h) / 2 + rotated_y;

                put_pixel(rotated_surface, rotated_x, rotated_y, pixel);
            }
        }
    } else // tan(pi/2) is undefined
    {
        for (int i = 0; i < src_surf_w; i++) {
            for (int j = 0; j < src_surf_h; j++) {
                pixel = get_pixel(src_surface, i, j);
                SDL_GetRGB(pixel, src_surface->format, &r, &g, &b);
                pixel = SDL_MapRGB(rotated_surface->format, r, g, b);

                x = i - half_src_surf_w;
                y = half_src_surf_h - j;

                //===================================================
                //****************Rotation-Formula 2*****************
                //===================================================

                // Shear 1
                rotated_x = round(x - y * tan(angle / 2));
                rotated_y = y;

                // Shear 2
                rotated_y = round(rotated_x * sin(angle) + rotated_y);

                // Shear 3
                rotated_x = round(rotated_x - rotated_y * tan(angle / 2));

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

void create_grids(int unsolved_sudoku[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE],
                  int solved_sudoku[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE]) {
    SDL_Surface *dflt_grid = NULL;
    SDL_Surface **given_numbers_surface =
        malloc(SUDOKU_GRID_SIZE * sizeof(SDL_Surface *));
    SDL_Surface **added_numbers_surface =
        malloc(SUDOKU_GRID_SIZE * sizeof(SDL_Surface *));

    char numbers[SUDOKU_GRID_SIZE][5] = {"1", "2", "3", "4", "5",
                                         "6", "7", "8", "9"};

    TTF_Font *numbers_font = TTF_OpenFont(GRID_NUMBER_FONT, 35);

    SDL_Rect position;

    // given numbers -> black color
    SDL_Color given_numbers_color = {.r = 0, .g = 0, .b = 0};

    // added numbers -> green color
    SDL_Color added_numbers_color = {.r = 0, .g = 154, .b = 23};

    for (int i = 0; i < SUDOKU_GRID_SIZE; i++) {
        given_numbers_surface[i] = TTF_RenderText_Blended(
            numbers_font, numbers[i], given_numbers_color);
        added_numbers_surface[i] = TTF_RenderText_Blended(
            numbers_font, numbers[i], added_numbers_color);
    }

    dflt_grid = load_image(SAVED_IMG_NAME_DG);

    for (int i = 0; i < SUDOKU_GRID_SIZE; i++) {
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++) {
            int current_nb = unsolved_sudoku[i][j];
            // printf("%d\n", current_nb);

            if (current_nb != 0) {
                // A constant has been added to the position due
                // to the white space generated in the font surface

                position.x = (dflt_grid->w / 18) * (1 + 2 * j) -
                             (given_numbers_surface[current_nb - 1]->w -
                              given_numbers_surface[current_nb - 1]->w / 4) /
                                 2;

                position.y = (dflt_grid->h / 18) * (1 + 2 * i) -
                             (given_numbers_surface[current_nb - 1]->h -
                              given_numbers_surface[current_nb - 1]->h / 3) /
                                 2;

                SDL_BlitSurface(given_numbers_surface[current_nb - 1], NULL,
                                dflt_grid, &position);
            }
        }
    }

    SDL_SaveBMP(dflt_grid, SAVED_IMG_NAME_UG);

    for (int i = 0; i < SUDOKU_GRID_SIZE; i++) {
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++) {
            int current_nb = solved_sudoku[i][j];
            // printf("%d\n", current_nb);

            if (unsolved_sudoku[i][j] == 0) {
                // A constant has been added to the position due
                // to the white space generated in the font surface

                position.x =
                    (dflt_grid->w / 18) * (1 + 2 * j) -
                    (added_numbers_surface[abs(current_nb - 1)]->w -
                     added_numbers_surface[abs(current_nb - 1)]->w / 4) /
                        2;

                position.y =
                    (dflt_grid->h / 18) * (1 + 2 * i) -
                    (added_numbers_surface[abs(current_nb - 1)]->h -
                     added_numbers_surface[abs(current_nb - 1)]->h / 3) /
                        2;

                SDL_BlitSurface(added_numbers_surface[abs(current_nb - 1)],
                                NULL, dflt_grid, &position);
            }
        }
    }

    SDL_SaveBMP(dflt_grid, SAVED_IMG_NAME_SG);

    SDL_FreeSurface(dflt_grid);
    for (int i = 0; i < SUDOKU_GRID_SIZE; i++) {
        SDL_FreeSurface(given_numbers_surface[i]);
        SDL_FreeSurface(added_numbers_surface[i]);
    }

    free(given_numbers_surface);
    free(added_numbers_surface);
}