# Makefile for the https://github.com/Nicolas-Reyland/OCR-Sudoku-Solver project

SHELL := /bin/bash
CFLAGS := -Wall -Wextra -Isrc
CC := gcc $(CFLAGS)

.PHONY: all
all: nn gui solver mem # src/nn/nn.o src/gui/gui.o src/solver/solver.o
	$(CC) -o main src/nn/nn.o src/gui/gui.o src/solver/solver.o

.PHONY: nn
nn: mem nn-model nn-data nn-session nn-utils nn-functions_descriptors
	$(CC) -c -o src/nn/nn.o src/nn/nn.c
#src/nn/model/model.o src/nn/data/nn_data_header.o src/nn/session/session.o src/nn/utils/nn_utils_header.o src/nn/functions_descriptor/nn_functions_descriptors_header.o

.PHONY: nn-model
nn-model: nn-model-layers
	$(CC) -c -o src/nn/model/model.o src/nn/model/model.c
# src/nn/model/layers/layer.o src/nn/model/layers/model_layer.o src/nn/model/layers/node.o

.PHONY: nn-model-layers
nn-model-layers: nn-utils-misc
	$(CC) -c -o src/nn/model/layers/layer.o src/nn/model/layers/layer.c
	$(CC) -c -o src/nn/model/layers/model_layers.o src/nn/model/layers/model_layers.c
	$(CC) -c -o src/nn/model/layers/node.o src/nn/model/layers/node.c

.PHONY: nn-functions_descriptors
nn-functions_descriptors:
	$(CC) -c -o src/nn/functions_descriptors/functions_descriptors_enums.o src/nn/functions_descriptors/functions_descriptors_enums.c

.PHONY: nn-data
nn-data: nn-data-sample
	$(CC) -c -o src/nn/data/data.o src/nn/data/data.c
	$(CC) -c -o src/nn/data/data_collection.o src/nn/data/data_collection.c
	$(CC) -c -o src/nn/data/init_data.o src/nn/data/init_data.c
	$(CC) -c -o src/nn/data/in_out_tuple.o src/nn/data/in_out_tuple.c

.PHONY: nn-data-sample
nn-data-sample: nn-utils-structs
	$(CC) -c -o src/nn/data/sample/sample.o src/nn/data/sample/sample.c

.PHONY: nn-session
nn-session:
	$(CC) -c -o src/nn/session/session.o src/nn/session/session.c

.PHONY: nn-utils
nn-utils: nn-utils-structs nn-utils-session nn-utils-misc
	@echo empty utils

.PHONY: nn-utils-structs
nn-utils-structs:
	$(CC) -c -o src/nn/utils/structs/shape_description.o src/nn/utils/structs/shape_description.c

.PHONY: nn-utils-misc
nn-utils-misc:
	$(CC) -c -o src/nn/utils/misc/randomness.o src/nn/utils/misc/randomness.c

.PHONY: nn-utils-session
nn-utils-functions: nn-utils-session-functions
	$(CC) -c -o src/nn/utils/session/evaluate.o src/nn/utils/session/evaluate.c

.PHONY: nn-utils-session-functions
nn-utils-functions-activations:
	$(CC) -c -o src/nn/utils/functions/losses.o src/nn/utils/functions/losses.c
	$(CC) -c -o src/nn/utils/functions/optimizers.o src/nn/utils/functions/optimizers.c
	$(CC) -c -o src/nn/utils/functions/activations.o src/nn/utils/functions/activations.c

.PHONY: mem
mem:
	$(CC) -c -o src/mem/linked_list.o src/mem/linked_list.c
	$(CC) -c -o src/mem/mem-management.o src/mem/mem-management.c


.PHONY: clean
clean:
	@./extra/make-clean.sh
