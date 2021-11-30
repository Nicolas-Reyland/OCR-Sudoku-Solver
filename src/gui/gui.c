#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <gtk/gtk.h>
#include "gui/image_process.h"
#include "nn/nn.h"
#include "grid_detection/headers/detect_grid.h"
#include "utils/mem/mem-management.h"

#include "utils/converter/converter.h"
#include "solver/headers/solver.h"


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define IMAGE_MAX_WIDTH 800
#define IMAGE_MAX_HEIGHT 600

#define SUDOKU_GRID_SIZE 9
#define OUTPUT_SIZE 9

#define PATH "./cells"

char *src_image_path = NULL;
nn_Model *number_prediction_model = NULL;

GdkPixbuf* create_pixbuf(const gchar *filename);
GdkPixbuf* resize_pixbuf(GdkPixbuf *pixbuf);
GtkWidget* gui_load_image(const gchar *filename);
void open_dialog(GtkWidget *widget, gpointer user_data);
void display_img_process_steps(GtkWidget *widget, gpointer user_data);
void display_rotated_image(GtkWidget *widget, gpointer user_data);
void launch_process(GtkWidget *widget, gpointer user_data);
void display_solution_grid(GtkWidget *widget, gpointer user_data);

int main(int argc, char **argv)
{
    // init Neural Network & Co.
    initRandom();
    initMemoryTracking();
    nn_Model* model = nn_loadModel("save/mnist/");
	number_prediction_model = model;


    // Window
    GtkWidget *window = NULL;

    // Frames
    GtkWidget *frame = NULL;

    // Boxes
    GtkWidget *main_box = NULL;
    GtkWidget *top_box = NULL;
    GtkWidget *center_right_buttons_box = NULL;
    GtkWidget *right_buttons_box = NULL;
    GtkWidget *rotation_wigets_box = NULL;
    GtkWidget *solution_buttons_box = NULL;
    GtkWidget *bottom_buttons_box = NULL;

    // Images
    GdkPixbuf *icon = NULL;
    GtkWidget *image = NULL;

    // Buttons
    GtkWidget *load_img_button = NULL;
    GtkWidget *apply_rotation_button = NULL;
    GtkWidget *solve_sudoku_button = NULL;
    GtkWidget *solution_button = NULL;
    GtkWidget *grayscale_img_button = NULL;
    GtkWidget *blurred_img_button = NULL;
    GtkWidget *binarised_img_button = NULL;

    // INPUTS
    GtkWidget *rotate_img_entry = NULL;

    // TOGGLES
    GtkWidget *brightness_check = NULL;

    // Separators
    GtkWidget *hseparator = NULL;

    // Variables that help display an image
    GtkWidget** widget_pointers[8] ={
                                    &window,
                                    &frame,
                                    &image,
                                    &rotate_img_entry,
                                    &solve_sudoku_button,
                                    &solution_button,
                                    &apply_rotation_button,
                                    &brightness_check};

    // Errors
    //GError *err = NULL;

    gtk_init(&argc, &argv);
    init_sdl();
    init_sdl_ttf();

    // Creates a GtkWindow
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Sets window title
    gtk_window_set_title(GTK_WINDOW(window), "OCR Sudoku solver");

    // Sets window default size
    gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

    // Sets window position
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Sets some border space around the edges of the window.
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    icon = create_pixbuf("gui_files/icon.jpg");
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    src_image_path = (char*)malloc((strlen("gui_files/no_image.png") + 1) * sizeof(char));
    //if (!src_image_path)
    //    exit(0);

    src_image_path = strcpy(src_image_path, "gui_files/no_image.png");

    image = gui_load_image(src_image_path);
    //image_process(src_image_path);

    // Box Creation
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    g_object_set(main_box, "margin", 20, NULL);

    top_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    center_right_buttons_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    right_buttons_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    rotation_wigets_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    solution_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    bottom_buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Frame creation
    frame = gtk_frame_new("Loaded Image");
    gtk_frame_set_label_align (GTK_FRAME(frame), 0.5, 1.0);
    //g_object_set(frame, "margin", 25, NULL);

    //
    gtk_widget_set_size_request(frame, round(IMAGE_MAX_WIDTH * 1.1),
                                    round(IMAGE_MAX_HEIGHT * 1.1));
    //

    gtk_container_add(GTK_CONTAINER(frame), image);

    // Buttons creation
    load_img_button = gtk_button_new_with_label("Load image");
    gtk_widget_set_tooltip_text(load_img_button, "Loads an image");

    apply_rotation_button = gtk_button_new_with_label("Apply");
    gtk_widget_set_tooltip_text(apply_rotation_button, "Applies the rotation (in degrees)");
    gtk_widget_set_sensitive(apply_rotation_button, FALSE);

    solve_sudoku_button = gtk_button_new_with_label("Solve Sudoku");
    gtk_widget_set_tooltip_text(solve_sudoku_button,
        "Launches the whole program and displays the non resolved grid");
    gtk_widget_set_sensitive(solve_sudoku_button, FALSE);

    solution_button = gtk_button_new_with_label("Solution");
    gtk_widget_set_tooltip_text(solution_button, "Displays the resolved grid");
    gtk_widget_set_sensitive(solution_button, FALSE);

    grayscale_img_button = gtk_button_new_with_label("Grayscale image");
    gtk_widget_set_tooltip_text(grayscale_img_button, "Displays the grayscale image");

    blurred_img_button = gtk_button_new_with_label("Blurred image");
    gtk_widget_set_tooltip_text(blurred_img_button, "Displays the blurred image");

    binarised_img_button = gtk_button_new_with_label("Binarised image");
    gtk_widget_set_tooltip_text(binarised_img_button, "Displays the binarised image");

    // Creates the horizontal separator
    hseparator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    // Inputs creation
    rotate_img_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(rotate_img_entry), "Enter angle degree");

    // Toggles creation
    brightness_check = gtk_check_button_new_with_label("Bright");
    gtk_widget_set_tooltip_text(brightness_check, "Tick if the image is too bright");

    // Sets the top box and bottom buttons box position in the 'main_box'
    gtk_box_pack_start(GTK_BOX(main_box), top_box, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), hseparator, FALSE, FALSE, 10);
    gtk_box_pack_end(GTK_BOX(main_box), bottom_buttons_box, FALSE, FALSE, 20);

    // Sets the image and right box position in the 'top_box'
    gtk_box_set_center_widget(GTK_BOX(top_box), frame);
    gtk_box_pack_end(GTK_BOX(top_box), center_right_buttons_box, FALSE, FALSE, 20);

    // Sets the right buttons box in the center
    gtk_box_set_center_widget(GTK_BOX(center_right_buttons_box), right_buttons_box);

    // Sets the right buttons position in the 'right_buttons_box'
    gtk_box_pack_start(GTK_BOX(right_buttons_box), load_img_button, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(right_buttons_box), rotation_wigets_box, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(right_buttons_box), solution_buttons_box, FALSE, FALSE, 10);

    // Sets the apply rotation entry and the rotation button to the 'rotation_wigets_box'
    gtk_box_pack_start(GTK_BOX(rotation_wigets_box), brightness_check, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(rotation_wigets_box), rotate_img_entry, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(rotation_wigets_box), apply_rotation_button, FALSE, FALSE, 5);

    // Sets the solution buttons position in the 'solution_buttons_box'
    gtk_box_pack_start(GTK_BOX(solution_buttons_box), solve_sudoku_button, TRUE, FALSE, 5);
    gtk_box_pack_end(GTK_BOX(solution_buttons_box), solution_button, TRUE, FALSE, 5);

    // Sets the bottom buttons position in the 'bottom_buttons_box'
    gtk_box_pack_start(GTK_BOX(bottom_buttons_box), grayscale_img_button, TRUE, FALSE, 0);
    gtk_box_set_center_widget(GTK_BOX(bottom_buttons_box), blurred_img_button);
    gtk_box_pack_end(GTK_BOX(bottom_buttons_box), binarised_img_button, TRUE, FALSE, 0);

    // Sets the action of the right buttons
    g_signal_connect(load_img_button, "clicked",
                    G_CALLBACK(open_dialog), widget_pointers);
    g_signal_connect(apply_rotation_button, "clicked",
                    G_CALLBACK(display_rotated_image), widget_pointers);

    g_signal_connect(solve_sudoku_button, "clicked", G_CALLBACK(launch_process), widget_pointers);
    g_signal_connect(solution_button, "clicked", G_CALLBACK(display_solution_grid), widget_pointers);

    // Sets the action of the bottom buttons
    g_signal_connect(grayscale_img_button, "clicked", G_CALLBACK(display_img_process_steps), widget_pointers);
    g_signal_connect(blurred_img_button, "clicked", G_CALLBACK(display_img_process_steps), widget_pointers);
    g_signal_connect(binarised_img_button, "clicked", G_CALLBACK(display_img_process_steps), widget_pointers);

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);

    //g_object_unref(icon);
    //g_object_unref(image_pixbuf);

    int unsolved_grid[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = {0};
    int solved_grid[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = {0};

    for (int i = 0; i < SUDOKU_GRID_SIZE; i++)
    {
        for (int j = 0; j < SUDOKU_GRID_SIZE; j++)
        {
            unsolved_grid[i][j] = rand() % 10;
            solved_grid[i][j] = (rand() % 9) + 1;
        }
    }

    create_grids((int**)unsolved_grid, (int**)solved_grid);

    /*
    GdkPixbuf *test_pixbuf = NULL;
    GError* test_error = NULL;
    test_pixbuf = create_pixbuf("sudoku_images_test/sudoku_test_6.jpeg");
    // Handle errors
    test_pixbuf = gdk_pixbuf_scale_simple(test_pixbuf,
           gdk_pixbuf_get_width(test_pixbuf) * 6, gdk_pixbuf_get_height(test_pixbuf) * 6, GDK_INTERP_BILINEAR);
    gdk_pixbuf_save(test_pixbuf, "resized_image.png", "png", &test_error, NULL);
    */

    gtk_main();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

// Creates a pixbuf from a filename
GdkPixbuf* create_pixbuf(const gchar *filename)
{
    GdkPixbuf *pixbuf = NULL;
    GError *error = NULL;

    pixbuf = gdk_pixbuf_new_from_file(filename, &error);

    if(!pixbuf)
    {
        fprintf(stderr, "Failed to create a pixbuf -> %s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

// Resizes a pixbuf
GdkPixbuf* resize_pixbuf(GdkPixbuf *pixbuf)
{
    if (pixbuf)
    {
        GdkPixbuf *resized_pixbuf = NULL;

        int width = gdk_pixbuf_get_width(pixbuf);
        int height = gdk_pixbuf_get_height(pixbuf);

        while(width > IMAGE_MAX_WIDTH || height > IMAGE_MAX_HEIGHT)
        {
            width *= 0.9;
            height *= 0.9;
        }

        resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf,
            width, height, GDK_INTERP_BILINEAR);

        // Clears a reference to the pixbuf object
        g_clear_object(&pixbuf);
        //

        return resized_pixbuf;
    }
    else
    {
        fprintf(stderr, "Failed to resize a pixbuf -> The given pixbuf is NULL\n");
        return pixbuf;
    }
}

GtkWidget* gui_load_image(const gchar *filename)
{
    GdkPixbuf *image_pixbuf = NULL;

    // Image loading in gui
    image_pixbuf = create_pixbuf(filename);
    // Handle errors

    image_pixbuf = resize_pixbuf(image_pixbuf);
    // Handle errors

    return gtk_image_new_from_pixbuf(image_pixbuf);
}

void open_dialog(GtkWidget *widget, gpointer user_data)
{
    //assert();
    GtkWidget ***widget_pointers = user_data;

    GtkWidget **window = widget_pointers[0];
    GtkWidget **frame = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];
    GtkWidget **solve_sudoku_button = widget_pointers[4];
    //GtkWidget **solution_button = widget_pointers[5];
    GtkWidget **apply_rotation_button = widget_pointers[6];

    GtkWidget *button = widget;

    GdkPixbuf *loaded_img_pixbuf = NULL;
    GtkWidget *dialog = NULL;
    GtkFileFilter *dialog_filter = NULL;

    //
    if (button == NULL)
        printf("open_dialog: No button is given in parameter\n");
    if (*window == NULL)
        printf("THE WINDOW POINTER IS NULL\n");

    if (*frame == NULL)
        printf("THE IMAGE POINTER IS NULL\n");

    if (*image == NULL)
        printf("THE IMAGE POINTER IS NULL\n");
    //

    dialog = gtk_file_chooser_dialog_new("Load an image",
        GTK_WINDOW(*window), GTK_FILE_CHOOSER_ACTION_OPEN,
                                    ("_Cancel"),
                                    GTK_RESPONSE_CANCEL,
                                    ("_Load"),
                                    GTK_RESPONSE_OK,
                                    NULL);


    // Dialog filter
    dialog_filter = gtk_file_filter_new();
    gtk_file_filter_set_name(dialog_filter, "Images");
    gtk_file_filter_add_mime_type(dialog_filter, "image/png");
    gtk_file_filter_add_mime_type(dialog_filter, "image/jpg");
    gtk_file_filter_add_mime_type(dialog_filter, "image/jpeg");
    gtk_file_filter_add_mime_type(dialog_filter, "image/bmp");

    gtk_file_filter_add_pattern(dialog_filter, "*.png");
    gtk_file_filter_add_pattern(dialog_filter, "*.jpg");
    gtk_file_filter_add_pattern(dialog_filter, "*.jpeg");
    gtk_file_filter_add_pattern(dialog_filter, "*.bmp");

    gtk_file_chooser_add_filter((GtkFileChooser*)dialog, dialog_filter);

    gtk_widget_show_all(dialog);
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK)
    {
        gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        loaded_img_pixbuf = create_pixbuf(filename);

        if (loaded_img_pixbuf)
        {
            // Finalizes the previous widget (image) and removes
            // it from the container
            gtk_widget_destroy(*image);

            free(src_image_path);
            src_image_path = (char*)malloc((strlen(filename) + 1) * sizeof(char));
            src_image_path = strcpy(src_image_path, filename);

            *image = gui_load_image(filename);
            //image_process(filename);

            gtk_container_add(GTK_CONTAINER(*frame), *image);
            //gtk_box_pack_start(GTK_BOX(*top_box), *image, TRUE, FALSE, 0);

            //gtk_widget_set_sensitive(*solution_button, FALSE);
            gtk_widget_set_sensitive(*apply_rotation_button, TRUE);
            gtk_widget_set_sensitive(*solve_sudoku_button, TRUE);

            gtk_widget_show_all(*window);
        }
    }
    /*
    else // response = GTK_RESPONSE_CANCEL
        printf("You pressed the cancel button\n");
    */

    gtk_widget_destroy(dialog);
}

void display_img_process_steps(GtkWidget *widget, gpointer user_data)
{
    GtkWidget ***widget_pointers = user_data;

    //assert();
    GtkWidget **window = widget_pointers[0];
    GtkWidget **frame = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];

    GtkWidget *button = widget;

    const gchar* button_label = gtk_button_get_label(GTK_BUTTON(button));

    if (strcmp(button_label, "Grayscale image") == 0)
    {
        if (create_pixbuf(SAVED_IMG_NAME_G))
        {
            gtk_widget_destroy(*image);

            *image = gui_load_image(SAVED_IMG_NAME_G);

            gtk_container_add(GTK_CONTAINER(*frame), *image);
            //gtk_box_pack_start(GTK_BOX(*frame), *image, TRUE, FALSE, 0);

            gtk_widget_show_all(*window);
        }
    }
    else if (strcmp(button_label, "Blurred image") == 0)
    {
        if (create_pixbuf(SAVED_IMG_NAME_BL))
        {
            gtk_widget_destroy(*image);

            *image = gui_load_image(SAVED_IMG_NAME_BL);

            gtk_container_add(GTK_CONTAINER(*frame), *image);
            //gtk_box_pack_start(GTK_BOX(*frame), *image, TRUE, FALSE, 0);

            gtk_widget_show_all(*window);
        }
    }
    else // button_label = "Binarised image"
    {
        if (create_pixbuf(SAVED_IMG_NAME_BI))
        {
            gtk_widget_destroy(*image);

            *image = gui_load_image(SAVED_IMG_NAME_BI);

            gtk_container_add(GTK_CONTAINER(*frame), *image);
            //gtk_box_pack_start(GTK_BOX(*frame), *image, TRUE, FALSE, 0);

            gtk_widget_show_all(*window);
        }
    }
}

void display_rotated_image(GtkWidget *widget, gpointer user_data)
{
    GtkWidget ***widget_pointers = user_data;

    //assert();
    GtkWidget **window = widget_pointers[0];
    GtkWidget **frame = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];
    GtkWidget **rotate_img_entry = widget_pointers[3];

    GtkWidget *apply_button = widget;

    if (apply_button == NULL)
        printf("display_rotated_image: No button is given in parameter\n");

    const char *angle = gtk_entry_get_text(GTK_ENTRY(*rotate_img_entry));

    char *endPtr = NULL;
    double value = strtod(angle, &endPtr);

    if (strlen(angle) > 0 && *endPtr == '\0')
    {
        value = fmod(value, 360.0);
        rotate_image(src_image_path, value);

        if (create_pixbuf(SAVED_IMG_NAME_R))
        {
            gtk_widget_destroy(*image);

            free(src_image_path);
            src_image_path = (char*)malloc((strlen(SAVED_IMG_NAME_R) + 1) * sizeof(char));
            src_image_path = strcpy(src_image_path, SAVED_IMG_NAME_R);

            *image = gui_load_image(SAVED_IMG_NAME_R);
            //image_process(SAVED_IMG_NAME_R);

            gtk_container_add(GTK_CONTAINER(*frame), *image);
            //gtk_box_pack_start(GTK_BOX(*frame), *image, TRUE, FALSE, 0);

            gtk_widget_show_all(*window);
        }
    }

}

int predictionToNumber(double* prediction)
{
	int max_index = 0;
	for (int i = 1; i < 9; i++) {
		if (prediction[i] > prediction[max_index]) {
			max_index = i;
		}
	}

	// mapping from 0-8 to 1-9 with this +1
	return max_index + 1;
}

void launch_process(GtkWidget *widget, gpointer user_data)
{
	nn_Model* model = number_prediction_model;
    GtkWidget ***widget_pointers = user_data;
    GtkWidget *solve_sudoku_button = widget;

    GtkWidget **window = widget_pointers[0];
    GtkWidget **frame = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];
    GtkWidget **solution_button = widget_pointers[5];
    GtkWidget **apply_rotation_button = widget_pointers[6];
    GtkWidget **brightness_check = widget_pointers[7];

    //===================================================
    //***************Image treatment part****************
    //===================================================

    printf("Started image treatment part.\n");
    image_process(src_image_path, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(*brightness_check)));
    printf("Finished image treatment part.\n");

    //===================================================
    //***************Grid detection part*****************
    //===================================================

    printf("Started grid detection part.\n");
    detect_grid(SAVED_IMG_NAME_BI);
    printf("Finished grid detection part.\n");

    //===================================================
    //********************OCR part***********************
    //===================================================

    int unsolved_grid[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = {0};
    int solved_grid[SUDOKU_GRID_SIZE][SUDOKU_GRID_SIZE] = {0};

    unsigned int nb_cells;

    //count the number of cells to process
    nb_cells = count_files(PATH); // , OUTPUT_SIZE);


    // initialize the arrays that we will use in order to
    // fill a matrix
    double** value_array = mem_calloc(nb_cells, sizeof(double*));
    Cell** positions_array = mem_calloc(nb_cells, sizeof(Cell*));

    //convert the cells img into a double array with their position
    converter(PATH, value_array, positions_array);

    // fills the matrix with the number we will guess
    // through the neural network
    for(unsigned int k = 0; k < nb_cells; k++)
    {
        double* prediction = model->use(model, value_array[k]);

        int x, y;
        x = positions_array[k]->x;
        y = positions_array[k]->y;

        // convert the double array into an integer
        // and stores it at the right place of the matrix
        unsolved_grid[y][x] = predictionToNumber(prediction);
        mem_free(prediction);
    }

    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            solved_grid[i][j] = unsolved_grid[i][j];

    //===================================================
    //****************Sudoku solver part*****************
    //===================================================

    //solve the new matrix
    solver(solved_grid);

    //free the content of the arrays that we don't use anymore
    for(unsigned int k = 0; k < nb_cells; k++)
    {
        mem_free(value_array[k]);
        mem_free(positions_array[k]);
    }

    //free the arrays
    mem_free(value_array);
    mem_free(positions_array);

    // the matrix that are outputed of this step are
    // unsolved_grid: the matrix that represents the sudoku grid before it is solved
    // solved_grid  : the matrix that represents the sudoku grid after it has been solved


    // Unsolved and solved image building
    create_grids((int**)unsolved_grid, (int**)solved_grid);

    // Verify if the process has succeeded
    gtk_widget_set_sensitive(*apply_rotation_button, FALSE);
    gtk_widget_set_sensitive(solve_sudoku_button, FALSE);
    gtk_widget_set_sensitive(*solution_button, TRUE);
    //

    gtk_widget_destroy(*image);

    *image = gui_load_image(SAVED_IMG_NAME_UG);
    gtk_container_add(GTK_CONTAINER(*frame), *image);

    gtk_widget_show_all(*window);
}

void display_solution_grid(GtkWidget *widget, gpointer user_data)
{
    GtkWidget ***widget_pointers = user_data;
    // Suppress warnings
    (void)widget;

    GtkWidget **window = widget_pointers[0];
    GtkWidget **frame = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];

    gtk_widget_destroy(*image);

    *image = gui_load_image(SAVED_IMG_NAME_SG);
    gtk_container_add(GTK_CONTAINER(*frame), *image);

    gtk_widget_show_all(*window);
}
