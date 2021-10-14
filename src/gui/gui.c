#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <gtk/gtk.h>
#include "image_process.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define IMAGE_MAX_WIDTH 800
#define IMAGE_MAX_HEIGHT 600

GdkPixbuf* create_pixbuf(const gchar *filename);
GdkPixbuf* resize_pixbuf(GdkPixbuf *pixbuf);
GtkWidget* gui_load_image(const gchar *filename);
void open_dialog(GtkWidget *button, GtkWidget** widget_pointers[]);

int main(int argc, char **argv)
{
    // Window
    GtkWidget *window = NULL;

    // Frames
    GtkWidget *frame = NULL;

    // Boxes
    GtkWidget *main_box = NULL;
    GtkWidget *buttons_box = NULL;

    GdkPixbuf *icon = NULL;
    //GdkPixbuf *image_pixbuf = NULL;

    // Images
    GtkWidget *image = NULL;

    // Buttons
    GtkWidget *ld_img_button = NULL;
    GtkWidget *rotate_button = NULL;
    GtkWidget *grayscale_button = NULL;

    // Variable that helps display an image
    GtkWidget** widget_pointers[3] = {&window, &main_box, &image};

    // File chooser dialog
    //GtkWidget *file_chooser_dialog = NULL;

    // Errors
    //GError *err = NULL;

    gtk_init(&argc, &argv);

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
    
    icon = create_pixbuf("gui_images/icon.jpg");
    gtk_window_set_icon(GTK_WINDOW(window), icon);

    // Frame creation
    frame = gtk_frame_new("Loaded Image");
    gtk_frame_set_label_align (GTK_FRAME(frame), 0.5, 1.0);

    g_object_set(frame, "margin", 25, NULL);
    gtk_container_add(GTK_CONTAINER(window), frame);

    // Box Creation
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(frame), main_box);

    buttons_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    // Buttons creation
    ld_img_button = gtk_button_new_with_label("Load Image");
    gtk_widget_set_tooltip_text(ld_img_button, "Loads an image");

    rotate_button = gtk_button_new_with_label("Rotate");
    gtk_widget_set_tooltip_text(rotate_button, "Rotates the image");

    grayscale_button = gtk_button_new_with_label("Grayscale");
    gtk_widget_set_tooltip_text(grayscale_button, "Grayscale the image");

    // Set the buttons position in the 'buttons_box'
    gtk_box_pack_start(GTK_BOX(buttons_box), ld_img_button, TRUE, FALSE, 0);
    gtk_box_set_center_widget(GTK_BOX(buttons_box), grayscale_button);
    gtk_box_pack_end(GTK_BOX(buttons_box), rotate_button, TRUE, FALSE, 0);

    image = gui_load_image("gui_images/no_image.png");

    gtk_box_pack_start(GTK_BOX(main_box), image, TRUE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(main_box), buttons_box, FALSE, FALSE, 20);

    //
    image_process("sudoku_images_test/sudoku_test_6.jpg");
    //

    //
    g_signal_connect(ld_img_button, "clicked", G_CALLBACK(open_dialog), widget_pointers);
    //  

    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    
    g_object_unref(icon);
    //g_object_unref(image_pixbuf);

    gtk_main();    

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
            width /= 2;
            height /= 2;
        }

        resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 
            width, height, GDK_INTERP_BILINEAR);

        // Free the source image
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

void open_dialog(GtkWidget *button, GtkWidget** widget_pointers[])
{
    //assert();

    GtkWidget **window = widget_pointers[0];
    GtkWidget **main_box = widget_pointers[1];
    GtkWidget **image = widget_pointers[2];
    
    GtkWidget *dialog = NULL;
    GtkFileFilter *dialog_filter = NULL;

    //
    if (*window == NULL)
        printf("THE WINDOW POINTER IS NULL");

    if (*main_box == NULL)
        printf("THE IMAGE POINTER IS NULL");

    if (*image == NULL)
        printf("THE IMAGE POINTER IS NULL");
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
        
        if (create_pixbuf(filename))
        {
            gtk_widget_destroy(*image);

            *image = gui_load_image(filename);

            gtk_box_pack_start(GTK_BOX(*main_box), *image, TRUE, FALSE, 0);

            gtk_widget_show_all(*window);
        }
    }
    else // response = GTK_RESPONSE_CANCEL
        printf("You pressed the cancel button\n");

    gtk_widget_destroy(dialog);
}