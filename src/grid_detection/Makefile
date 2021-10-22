# -*-Makefile-*-

#target : dependencies
#	action

# Setting the compiler and the default linker program
CC = gcc
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS =
# main compilation options
CFLAGSNE = -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2`
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 `pkg-config --cflags sdl2`
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image

all : main

main : main.o detect_grid.o pixel_operations.o 
	$(CC) $(CFLAGSNE) main.o detect_grid.o pixel_operations.o -o main $(LDLIBS)

main.o : main.c
	$(CC) -c $(CFLAGSNE) main.c -o main.o $(LDLIBS)

detect_grid.o : detect_grid.c
	$(CC) -c $(CFLAGSNE) detect_grid.c -o detect_grid.o $(LDLIBS)

pixel_operations.o : pixel_operations.c
	$(CC) -c $(CFLAGSNE) pixel_operations.c -o pixel_operations.o $(LDLIBS)

clean :
	rm *.o main *.bmp

