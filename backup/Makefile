# Makefile for the https://github.com/Nicolas-Reyland/OCR-Sudoku-Solver project

SHELL := /bin/bash
CFLAGS := -std=c99 -O1 -Wall -Wextra -Isrc
CFLAGS_GUI := -Wall -Wextra -std=c99 -O1 `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0` -Isrc
LDLIBS_NN = -lm
LDLIBS_GUI = -lm `pkg-config --libs sdl2` -lSDL2_image -lSDL2_ttf `pkg-config --libs gtk+-3.0`
CC := gcc

# use recursive assignment (=) for this, not simple assignment (:=)
NN_SOURCES = $(shell find src/nn -name '*.c')
GUI_SOURCES = $(shell find src/nn -name '*.c')
GRID_DETECTION_SOURCES = $(shell find src/nn -name '*.c')
UTILS_SOURCES = $(shell find src/nn -name '*.c')

.PHONY: all
all: nn gui grid_detection solver utils
	@echo "all rule called (nothing to do as of yet)"

#main : nn
#	$(CC) $(CFLAGS) -o main test-train-save.c $(NN_SOURCES) $(UTILS_SOURCES) $(LDLIBS_NN)

# ------- Neural Networks -------
.PHONY: nn
nn: utils nn-model nn-data nn-session nn-utils nn-functions_descriptors
	$(CC) $(CFLAGS) -c -o src/nn/nn.o src/nn/nn.c

.PHONY: nn-model
nn-model: nn-model-layers
	$(CC) $(CFLAGS) -c -o src/nn/model/model.o src/nn/model/model.c
	$(CC) $(CFLAGS) -c -o src/nn/model/use_model.o src/nn/model/use_model.c
	$(CC) $(CFLAGS) -c -o src/nn/model/load_model.o src/nn/model/load_model.c
	$(CC) $(CFLAGS) -c -o src/nn/model/save_model.o src/nn/model/save_model.c

.PHONY: nn-model-layers
nn-model-layers: nn-model-nodes nn-utils-misc
	$(CC) $(CFLAGS) -c -o src/nn/model/layers/layer.o src/nn/model/layers/layer.c

.PHONY: nn-model-nodes
nn-model-nodes:
	$(CC) $(CFLAGS) -c -o src/nn/model/layers/node/node.o src/nn/model/layers/node/node.c

.PHONY: nn-functions_descriptors
nn-functions_descriptors:
	$(CC) $(CFLAGS) -c -o src/nn/functions_descriptors/functions_descriptors_enums.o src/nn/functions_descriptors/functions_descriptors_enums.c

.PHONY: nn-data
nn-data: nn-data-sample
	$(CC) $(CFLAGS) -c -o src/nn/data/data.o src/nn/data/data.c
	$(CC) $(CFLAGS) -c -o src/nn/data/data_collection.o src/nn/data/data_collection.c
	$(CC) $(CFLAGS) -c -o src/nn/data/init_data.o src/nn/data/init_data.c
	$(CC) $(CFLAGS) -c -o src/nn/data/in_out_tuple.o src/nn/data/in_out_tuple.c

.PHONY: nn-data-sample
nn-data-sample: nn-utils-structs
	$(CC) $(CFLAGS) -c -o src/nn/data/sample/sample.o src/nn/data/sample/sample.c

.PHONY: nn-session
nn-session:
	$(CC) $(CFLAGS) -c -o src/nn/session/session.o src/nn/session/session.c

.PHONY: nn-utils
nn-utils: nn-utils-structs nn-utils-session nn-utils-misc

.PHONY: nn-utils-structs
nn-utils-structs:
	$(CC) $(CFLAGS) -c -o src/nn/utils/structs/shape_description.o src/nn/utils/structs/shape_description.c

.PHONY: nn-utils-misc
nn-utils-misc:
	$(CC) $(CFLAGS) -c -o src/nn/utils/misc/randomness.o src/nn/utils/misc/randomness.c

.PHONY: nn-utils-session
nn-utils-functions: nn-utils-session-functions
	$(CC) $(CFLAGS) -c -o src/nn/utils/session/evaluate.o src/nn/utils/session/evaluate.c

.PHONY: nn-utils-session-functions
nn-utils-session-functions:
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/losses.o src/nn/utils/functions/losses.c
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/optimizers.o src/nn/utils/functions/optimizers.c
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/activations.o src/nn/utils/functions/activations.c $(LDLIBS_NN)
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/derivative.o src/nn/utils/functions/derivative.c $(LDLIBS_NN)


# ------- GUI & Image Processing -------
gui : src/gui/gui.o src/gui/image_process.o src/gui/pixel_operations.o nn utils grid_detection
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -o src/gui/gui src/gui/gui.o src/gui/image_process.o src/gui/pixel_operations.o $(NN_SOURCES) $(UTILS_SOURCES) $(GRID_DETECTION_SOURCES) $(LDLIBS_GUI) $(LDLIBS_NN)

src/gui/gui.o : src/gui/gui.c
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c src/gui/gui.c -o src/gui/gui.o $(LDLIBS_GUI)

src/gui/image_process.o : src/gui/image_process.c
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c src/gui/image_process.c -o src/gui/image_process.o $(LDLIBS_GUI)

src/gui/pixel_operations.o : src/gui/pixel_operations.c
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c src/gui/pixel_operations.c -o src/gui/pixel_operations.o $(LDLIBS_GUI)

gui_clean :
	rm src/gui/*.o src/gui/gui src/gui/grayscale.bmp src/gui/blurred_image.bmp src/gui/binarised_image.bmp src/gui/rotated_image.bmp


# ------- GUI & Image Processing -------
.PHONY: grid_detection
grid_detection:
	@echo "test"


# ------- Solver -------
.PHONY: solver
solver:
	$(CC) $(CFLAGS) -o src/solver/main src/solver/{main,sources/{reader,solver,tester,writer}}.c


# ------- Utils -------
.PHONY: utils
utils: utils-verbosity utils-converter-cell
	$(CC) $(CFLAGS) -c -o src/utils/mem/linked_list.o src/utils/mem/linked_list.c
	$(CC) $(CFLAGS) -c -o src/utils/mem/mem-management.o src/utils/mem/mem-management.c

.PHONY: utils-verbosity
utils-verbosity:
	$(CC) $(CFLAGS) -c -o src/utils/verbosity/verbose.o src/utils/verbosity/verbose.c
	$(CC) $(CFLAGS) -c -o src/utils/verbosity/progressbar.o src/utils/verbosity/progressbar.c

.PHONY:
utils-converter-cell:
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c -o src/utils/converter/converter.o src/utils/converter/converter.c $(LDLIBS_GUI)
	$(CC) $(CFLAGS) $(CFLAGS_GUI) -c -o src/utils/converter/sdl_operations.o src/utils/converter/sdl_operations.c $(LDLIBS_GUI)
	$(CC) $(CFLAGS) -c -o src/utils/cell/cell.o src/utils/cell/cell.c


# ------- Test Framework -------
.PHONY: test
test:
	@./extra/run-tests

.PHONY: clean-test
clean-test:
	@rm -rf /tmp/ocr-tests{,-perm}


# ------- Misc Rules -------
.PHONY: clean
clean:
	@./extra/make-clean.sh

