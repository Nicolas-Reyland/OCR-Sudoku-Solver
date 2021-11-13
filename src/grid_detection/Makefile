#-*-Makefile-*-

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

main : sources/main.o sources/detect_grid.o sources/pixel_operations.o ../utils/mem/linked_list.o sources/set.o
	$(CC) $(CFLAGSNE) sources/main.o sources/detect_grid.o sources/pixel_operations.o ../utils/mem/linked_list.o sources/set.o -o main $(LDLIBS)

main.o : sources/main.c
	$(CC) -c $(CFLAGSNE) sources/main.c -o sources/main.o $(LDLIBS)

sources/detect_grid.o : sources/detect_grid.c
	$(CC) -c $(CFLAGSNE) sources/detect_grid.c -o sources/detect_grid.o $(LDLIBS)

sources/pixel_operations.o : sources/pixel_operations.c
	$(CC) -c $(CFLAGSNE) sources/pixel_operations.c -o sources/pixel_operations.o $(LDLIBS)

../utils/mem/link_list.o : ../utils/mem/link_list.c
	$(CC) -c $(CFLAGSNE) ../utils/mem/link_list.c -o ../utils/mem/link_list.o $(LDLIBS)

sources/set.o : sources/set.c
	$(CC) -c $(CFLAGSNE) sources/set.c -o sources/set.o $(LDLIBS)



clean :
	rm cells/* sources/*.o main *.bmp ../utils/mem/*.o

