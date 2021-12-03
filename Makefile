# -*-Makefile-*-

SHELL := bash
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
# main compilation options
CFLAGS = -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -Isrc
# libs and path for linker
LDLIBS = -lm `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf `pkg-config --libs gtk+-3.0`

# ------- Main -------
%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@ $(LDLIBS)

$(EXEC) : $(OBJ)
	$(CC) $(CFLAGS) $^ -o $(EXEC) $(LDLIBS)

all : $(EXEC)

clean :
	@bash extra/make-clean.sh

# ------- Test Framework -------
.PHONY: test
test:
	@bash extra/run-tests

.PHONY: clean-test
clean-test:
	@rm -rf /tmp/ocr-tests{,-perm}

