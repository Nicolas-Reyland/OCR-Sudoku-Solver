# Makefile for the https://github.com/Nicolas-Reyland/OCR-Sudoku-Solver project

SHELL := /bin/bash
CFLAGS := -std=c99 -O1 -Wall -Wextra -Isrc
CFLAGS_GUI := `pkg-config --cflags sdl2` `pkg-config --cflags gtk+-3.0`
LDLIBS_NN = -lm
LDLIBS_GUI = -lm `pkg-config --libs sdl2` -lSDL2_image `pkg-config --libs gtk+-3.0`
CC := gcc

.PHONY: all
all: utils nn gui solver # src/nn/nn.o src/gui/gui.o src/solver/solver.o
	@echo "all rule called (nothing to do as of yet)"

# ------- Neural Networks -------
.PHONY: nn
nn: utils nn-model nn-data nn-session nn-utils nn-functions_descriptors
	$(CC) $(CFLAGS) -c -o src/nn/nn.o src/nn/nn.c

.PHONY: nn-model
nn-model: nn-model-layers
	$(CC) $(CFLAGS) -c -o src/nn/model/model.o src/nn/model/model.c

.PHONY: nn-model-layers
nn-model-layers: nn-model-layers-node nn-utils-structs
	$(CC) $(CFLAGS) -c -o src/nn/model/layers/layer.o src/nn/model/layers/layer.c

.PHONY: nn-model-layers-node
nn-model-layers-node: nn-utils-misc
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
	$(CC) $(CFLAGS) -c -o src/nn/data/in_out_tuple_linked_list.o src/nn/data/in_out_tuple_linked_list.c

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
	$(CC) $(CFLAGS) -c -o src/nn/utils/structs/dataset.o src/nn/utils/structs/dataset.c

.PHONY: nn-utils-misc
nn-utils-misc:
	$(CC) $(CFLAGS) -c -o src/nn/utils/misc/randomness.o src/nn/utils/misc/randomness.c

.PHONY: nn-utils-session
nn-utils-functions: nn-utils-session-functions
	$(CC) $(CFLAGS) -c -o src/nn/utils/session/evaluate.o src/nn/utils/session/evaluate.c

.PHONY: nn-utils-session-functions
nn-utils-functions-activations:
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/losses.o src/nn/utils/functions/losses.c
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/optimizers.o src/nn/utils/functions/optimizers.c
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/activations.o src/nn/utils/functions/activations.c $(LDLIBS_NN)
	$(CC) $(CFLAGS) -c -o src/nn/utils/functions/derivation.o src/nn/utils/functions/derivation.c $(LDLIBS_NN)


# ------- GUI & Image Processing -------
gui : utils src/gui/gui.o src/gui/image_process.o src/gui/pixel_operations.o
	$(CC) $(CFLAGS_GUI) src/gui/gui.o src/gui/image_process.o src/gui/pixel_operations.o -o src/gui/gui $(LDLIBS_GUI)

src/gui/gui.o : src/gui/gui.c
	$(CC) -c $(CFLAGS_GUI) src/gui/gui.c -o src/gui/gui.o $(LDLIBS_GUI)

src/gui/image_process.o : src/gui/image_process.c
	$(CC) -c $(CFLAGS_GUI) src/gui/image_process.c -o src/gui/image_process.o $(LDLIBS_GUI)

src/gui/pixel_operations.o : src/gui/pixel_operations.c
	$(CC) -c $(CFLAGS_GUI) src/gui/pixel_operations.c -o src/gui/pixel_operations.o $(LDLIBS_GUI)

# ------- Utils -------
.PHONY: utils
utils: utils-mem utils-verbosity

.PHONY: utils-mem
utils-mem:
	$(CC) $(CFLAGS) -c -o src/utils/mem/linked_list.o src/utils/mem/linked_list.c
	$(CC) $(CFLAGS) -c -o src/utils/mem/mem-management.o src/utils/mem/mem-management.c

.PHONY: utils-verbosity
utils-verbosity:
	$(CC) $(CFLAGS) -c -o src/utils/verbosity/verbose.o src/utils/verbosity/verbose.c

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
