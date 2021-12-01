# -*-Makefile-*-

#target : dependencies
#	action

# Setting the compiler and the default linker program
CC = gcc
# exec
EXEC = gui
# all c source files
SRC = $(wildcard *.c) $(wildcard ../grid_detection/*.c)
# all object files
OBJ = $(SRC:.c=.o)
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS =
# main compilation options
CFLAGSNE = -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -I ../ -I ../grid_detection/headers
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -I ../ -I ../grid_detection/headers
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf `pkg-config --libs gtk+-3.0`

%.o : %.c
	$(CC) -c $(CFLAGSNE) $< -o $@ $(LDLIBS)

%.o : %../grid_detection/.c
	$(CC) -c $(CFLAGSNE) $< -o $@ $(LDLIBS)

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGSNE) $^ -o $(EXEC) $(LDLIBS)

all : $(EXEC)

clean :
	rm *.o $(EXEC) *.bmp ../grid_detection/*.o cases/