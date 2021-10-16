# -*-Makefile-*-

#target : dependencies
#	action

# Setting the compiler and the default linker program
CC = gcc
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS =
# main compilation options
CFLAGSNE = -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -I..
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -I..
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image `pkg-config --libs gtk+-3.0`

all : gui

gui : gui.o image_process.o pixel_operations.o
	$(CC) $(CFLAGSNE) gui.o image_process.o pixel_operations.o -o gui $(LDLIBS)

gui.o : gui.c
	$(CC) -c $(CFLAGSNE) gui.c -o gui.o $(LDLIBS)

image_process.o : image_process.c
	$(CC) -c $(CFLAGSNE) image_process.c -o image_process.o $(LDLIBS)

pixel_operations.o : pixel_operations.c
	$(CC) -c $(CFLAGSNE) pixel_operations.c -o pixel_operations.o $(LDLIBS)

clean :
	rm *.o gui grayscale.bmp blurred_image.bmp binarised_image.bmp rotated_image.bmp