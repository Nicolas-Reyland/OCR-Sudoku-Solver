# Makefile for the https://github.com/Nicolas-Reyland/OCR-Sudoku-Solver project

SHELL := /bin/bash
CFLAGS := -Wall -Wextra
CC := gcc $(CFLAGS)

.PHONY: all
all: nn gui solver # src/nn/nn.o src/gui/gui.o src/solver/solver.o
	$(CC) -o main src/nn/nn.o src/gui/gui.o src/solver/solver.o

.PHONY: nn
nn: nn-model nn-data nn-session nn-utils nn-functions_descriptors
	$(CC) -c -o src/nn/nn.o src/nn/model/model.o src/nn/data/nn_data_header.o src/nn/session/session.o src/nn/utils/nn_utils_header.o src/nn/functions_descriptor/nn_functions_descriptors_header.o

.PHONY: nn-model
nn-model: nn-model-layers
	$(CC) -c -o src/nn/model/model.o src/nn/model/model.c src/nn/model/layers/layer.o src/nn/model/layers/model_layer.o src/nn/model/layers/node.o

.PHONY: nn-model-layers
nn-model-layers:
	$(CC) -c -o src/nn/model/layers/layer.o src/nn/model/layers/layer.c
	$(CC) -c -o src/nn/model/layers/model_layer.o src/nn/model/layers/model_layer.c
	$(CC) -c -o src/nn/model/layers/node.o src/nn/model/layers/node.c

.PHONY: nn-data
nn-data:
	$(CC) -c -o src/nn/data/nn_data_header.o src/nn/data/data.c src/nn/data/data_collection.c

.PHONY: nn-session
nn-session:
	$(CC) -c -o src/nn/session/session.o src/nn/session/session.c

.PHONY: nn-utils
nn-utils: nn-utils-structs nn-utils-functions
	$(CC) -c -o src/nn/utils/nn_utils_header.o src/nn/utils/functions/optimizers.o src/nn/utils/functions/losses.o src/nn/utils/functions/activations.o src/nn/utils/structs/shape_description.o

.PHONY: nn-utils-structs
nn-utils-structs:
	$(CC) -c -o src/nn/utils/structs/shape_description.o src/nn/utils/structs/shape_description.c

.PHONY: nn-utils-functions
nn-utils-functions:
	$(CC) -c -o src/nn/utils/functions/optimizers.o src/nn/utils/functions/optimizers.c
	$(CC) -c -o src/nn/utils/functions/losses.o src/nn/utils/functions/losses.c
	$(CC) -c -o src/nn/utils/functions/activations.o src/nn/utils/functions/activations.c

.PHONY: clean
clean:
	if [ -e main ]; then
		rm -f main
	fi
	for object_file in $(find . -type f -name "*.o")
	do
		rm -f "$objetc_file"
	done
