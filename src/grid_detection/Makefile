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

main : main.o read_img.o pixel_operation.o square_detection.o
	$(CC) $(CFLAGSNE) main.o read_img.o pixel_operation.o square_detection.o -o main $(LDLIBS)

main.o : main.c
	$(CC) -c $(CFLAGSNE) main.c -o main.o $(LDLIBS)

read_img.o : read_img.c
	$(CC) -c $(CFLAGSNE) read_img.c -o read_img.o $(LDLIBS)

pixel_operation.o : pixel_operation.c
	$(CC) -c $(CFLAGSNE) pixel_operation.c -o pixel_operation.o $(LDLIBS)

square_detection.o : square_detection.c
	$(CC) -c $(CFLAGSNE) square_detection.c -o square_detection.o $(LDLIBS)

clean :
	rm *.o main
