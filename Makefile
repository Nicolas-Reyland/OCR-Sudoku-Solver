# -*-Makefile-*-

#target : dependencies
#	action
SHELL := /bin/bash
# Setting the compiler and the default linker program
CC = gcc
# exec
EXEC = main
# all c source files
#SRC = $(wildcard *.c) $(wildcard src/grid_detection/*.c)
ALL_SRC = $(shell find src/{gui,nn,utils} -name '*.c') $(wildcard src/solver/sources/*.c) $(wildcard src/grid_detection/sources/*.c)
SRC = $(filter-out src/grid_detection/sources/main.c,$(ALL_SRC))
# all object files
OBJ = $(SRC:.c=.o)
# options for pre-processor (-I, -include, -D ... )
CPPFLAGS =
# main compilation options
CFLAGSNE = -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -Isrc
CFLAGS = -Wall -Wextra -Werror -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -Isrc
# Linker options (probably always empty)
LDFLAGS =
# libs and path for linker
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf `pkg-config --libs gtk+-3.0` -lSDL2_gfx

%.o : %.c
	$(CC) -c $(CFLAGSNE) $< -o $@ $(LDLIBS)

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGSNE) $^ -o $(EXEC) $(LDLIBS)

all : $(EXEC)

clean :
	./extra/make-clean.sh

# ------- Test Framework -------
.PHONY: test
test:
	@./extra/run-tests

.PHONY: clean-test
clean-test:
	@rm -rf /tmp/ocr-tests{,-perm}
